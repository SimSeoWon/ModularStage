#include "Manager_Mission.h"
#include "Subsystems/SubsystemCollection.h"
#include "UObject/UObjectIterator.h"

#include "ModularStage/Mission/TaskSystem/MissionTaskExecutor.h"

UManager_Mission* UManager_Mission::Get() 
{
    return GetManager<UManager_Mission>();
}

void UManager_Mission::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UManager_Mission::Deinitialize()
{
	// 모든 실행기 안전하게 종료
	for (auto& Executor : MainMissionExecutors)
	{
		if (IsValid(Executor))
		{
			Executor->Shutdown();
		}
	}

	for (auto& Executor : SubMissionExecutors)
	{
		if (IsValid(Executor))
		{
			Executor->Shutdown();
		}
	}

	MainMissionExecutors.Empty();
	SubMissionExecutors.Empty();
	CurrentMainMissionOrder = 0;

	Super::Deinitialize();
}

bool UManager_Mission::ShouldCreateSubsystem(UObject* Outer) const
{
    //UClass
    for (TObjectIterator<UClass> It; It; ++It)
    {
        if (It->IsChildOf(UManager_Mission::StaticClass()) && (*It != UManager_Mission::StaticClass()))
        {

            return false;
        }
    }

    return true;
}

void UManager_Mission::RegisterExecutor(TObjectPtr<UMissionTaskExecutor> ExecutorToRegister)
{
	if (IsValid(ExecutorToRegister))
	{
		// 미션 타입에 따라 적절한 리스트에 추가
		if (ExecutorToRegister->GetMissionType() == EMissionType::Main)
		{
			if (!MainMissionExecutors.Contains(ExecutorToRegister))
			{
				MainMissionExecutors.Add(ExecutorToRegister);
				
				// 실행 순서(ExecutionOrder)에 따라 정렬
				MainMissionExecutors.Sort([](const TObjectPtr<UMissionTaskExecutor>& A, const TObjectPtr<UMissionTaskExecutor>& B)
				{
					return A->GetExecutionOrder() < B->GetExecutionOrder();
				});
			}
		}
		else
		{
			if (!SubMissionExecutors.Contains(ExecutorToRegister))
			{
				SubMissionExecutors.Add(ExecutorToRegister);
			}
		}

		// 델리게이트 구독
		ExecutorToRegister->OnStepChanged.AddUObject(this, &UManager_Mission::HandleOnStepChanged);
		ExecutorToRegister->OnCompleted.AddUObject(this, &UManager_Mission::HandleOnCompleted);

		// 미션 타입에 따른 실행 제어
		if (ExecutorToRegister->GetMissionType() == EMissionType::Sub)
		{
			// 서브 미션은 즉시 병렬 실행
			ExecutorToRegister->StartMission();
			UE_LOG(LogTemp, Log, TEXT("Manager_Mission: Sub Mission [%s] Started Immediately (Parallel)"), *ExecutorToRegister->GetName());
		}
		else if (ExecutorToRegister->GetMissionType() == EMissionType::Main)
		{
			// 메인 미션은 순서 확인 후 직렬 실행
			if (ExecutorToRegister->GetExecutionOrder() == CurrentMainMissionOrder)
			{
				ExecutorToRegister->StartMission();
				UE_LOG(LogTemp, Log, TEXT("Manager_Mission: Main Mission [%s] Started (Order: %d)"), *ExecutorToRegister->GetName(), CurrentMainMissionOrder);
			}
			else
			{
				UE_LOG(LogTemp, Log, TEXT("Manager_Mission: Main Mission [%s] Registered but Waiting (Current Order: %d, Mission Order: %d)"), 
					*ExecutorToRegister->GetName(), CurrentMainMissionOrder, ExecutorToRegister->GetExecutionOrder());
			}
		}
	}
}

void UManager_Mission::NotifyTaskExecutorCompletion(TObjectPtr<UMissionTaskExecutor> CompletedExecutor)
{
    if (IsValid(CompletedExecutor))
    {
        UE_LOG(LogTemp, Log, TEXT("UMissionTaskExecutor Completed: %s"), *CompletedExecutor->GetName());
        //MissionExecutors.Remove(CompletedExecutor); // 이 라인을 추가합니다.
    }
}

void UManager_Mission::HandleOnStepChanged(UMissionTaskExecutor* Executor, int32 Step)
{
	if (IsValid(Executor))
	{
		UE_LOG(LogTemp, Log, TEXT("Manager_Mission: Executor [%s] Step Changed to: %d"), *Executor->GetName(), Step);
	}
}

void UManager_Mission::HandleOnCompleted(UMissionTaskExecutor* Executor)
{
	if (IsValid(Executor))
	{
		UE_LOG(LogTemp, Log, TEXT("Manager_Mission: Executor [%s] All Tasks Completed!"), *Executor->GetName());
		
		if (Executor->GetMissionType() == EMissionType::Main)
		{
			// 메인 미션이 완료되었으므로 다음 메인 미션 진행
			CurrentMainMissionOrder++;
			UE_LOG(LogTemp, Log, TEXT("Manager_Mission: Advancing Main Mission Order to: %d"), CurrentMainMissionOrder);
			TryStartNextMainMission();
		}

		// 필요한 경우 여기서 리스트에서 제거하거나 전역 후처리 수행
	}
}

void UManager_Mission::TryStartNextMainMission()
{
	for (auto& Executor : MainMissionExecutors)
	{
		if (IsValid(Executor) && 
			!Executor->IsActive() && 
			Executor->GetExecutionOrder() == CurrentMainMissionOrder)
		{
			Executor->StartMission();
			UE_LOG(LogTemp, Log, TEXT("Manager_Mission: Next Main Mission [%s] Started (Order: %d)"), *Executor->GetName(), CurrentMainMissionOrder);
			return; // 한 번에 하나의 메인 미션만 실행
		}
	}
}

void UManager_Mission::OnPreLoadMap(const FString& string)
{
	Super::OnPreLoadMap(string);

	// 맵 로드 시 이전 미션 상태 초기화
	MainMissionExecutors.Empty();
	SubMissionExecutors.Empty();
	CurrentMainMissionOrder = 0;

	UE_LOG(LogTemp, Log, TEXT("Manager_Mission: Mission state reset due to level transition."));
}
