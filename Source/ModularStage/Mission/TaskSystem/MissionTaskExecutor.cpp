#include "MissionTaskExecutor.h"
#include "MissionTaskTable.h"
#include "Table/TableEnum.h"
#include "Manager/Manager_Mission.h"

#include "MissionTaskBase.h"
#include "MissionTask_Spawn.h"
#include "MissionTask_ObjectSpawn.h"
#include "MissionMultiTaskBase.h"
#include "MissionTask_SetCamera.h"
#include "MissionTask_PlayDialog.h"

UMissionTaskExecutor::UMissionTaskExecutor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CurrentState = EMissionExecutorState::None;
	Step = 0;
}

void UMissionTaskExecutor::Initialize(const FDataTableRowHandle& InMissionData)
{
	MissionData = InMissionData;
	SetState(EMissionExecutorState::Ready);
}

void UMissionTaskExecutor::Shutdown()
{
	SetState(EMissionExecutorState::None);
	SequentialTaskList.Empty();
	OccupiedTask = nullptr;
}

void UMissionTaskExecutor::StartMission()
{
	SetState(EMissionExecutorState::Running);
}

void UMissionTaskExecutor::SetState(EMissionExecutorState InState)
{
	if (CurrentState == InState)
	{
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("MissionTaskExecutor [%s] State Changed: %d -> %d"), 
		*GetName(), (int32)CurrentState, (int32)InState);

	CurrentState = InState;
}

TStatId UMissionTaskExecutor::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UMissionTaskExecutor, STATGROUP_Tickables);
}

bool UMissionTaskExecutor::IsTickable() const
{
	return CurrentState == EMissionExecutorState::Running && IsValid(this) && !HasAnyFlags(RF_BeginDestroyed | RF_FinishDestroyed);
}

bool UMissionTaskExecutor::InitializeFromTaskList(const TArray<FMissionTaskInfo>& InTaskList)
{
	// Clear any previous tasks
	SequentialTaskList.Empty();
	OccupiedTask = nullptr;
	Step = 0;
	SetState(EMissionExecutorState::None);

	if (InTaskList.IsEmpty())
	{
		return false;
	}

	for (const FMissionTaskInfo& TaskInfo : InTaskList)
	{
		TObjectPtr<UMissionTaskBase> NewTask = CreateTask(TaskInfo);
		if (NewTask)
		{
			SequentialTaskList.Add(NewTask);
		}
		else
		{
			///에러 로그 하나 남겨주기.
			UE_LOG(LogTemp, Warning, TEXT("UMissionTaskExecutor::InitializeFromTaskList - Failed to create task for type: %d"), (int32)TaskInfo.Type);
		}
	}

	SetState(EMissionExecutorState::Ready);
	return true;
}

void UMissionTaskExecutor::Tick(float DeltaTime)
{
	if (!IsTickable())
	{
		return;
	}

	// 현재 처리중인 태스크가 없으면 다음 태스크를 가져옵니다.
	if (false == IsValid(OccupiedTask))
	{
		if (SequentialTaskList.IsValidIndex(Step))
		{
			OccupiedTask = SequentialTaskList[Step];
		}
		else
		{
			// 모든 태스크 완료 (혹은 더 이상 태스크가 없음)
			SetState(EMissionExecutorState::Completed);

			OnCompleted.Broadcast(this);

			if (UManager_Mission* TaskManager = UManager_Mission::Get())
			{
				TaskManager->NotifyTaskExecutorCompletion(this); // 현재 UMissionTaskExecutor 인스턴스를 전달
			}
			return;
		}
	}

	// 현재 태스크가 유효하면 DoAction을 호출합니다.
	if (IsValid(OccupiedTask))
	{
		ETaskExecutorResult Result = OccupiedTask->DoAction();
		if (Result == ETaskExecutorResult::Progress)
		{
			// 태스크가 아직 진행 중
			return;
		}
		else if (Result == ETaskExecutorResult::Fail)
		{
			/// 태스크가 실패했다!
			TObjectPtr<UMissionTaskBase> prevOccupiedTask = OccupiedTask;
			--Step;
			if (0 >= Step)
			{
				Step = 0;
			}

			OnStepChanged.Broadcast(this, Step);

			if (SequentialTaskList.IsValidIndex(Step))
			{
				OccupiedTask = SequentialTaskList[Step];
			}

			if (IsValid(OccupiedTask))
			{
				OccupiedTask->RollBack();
			}

			if (OccupiedTask != prevOccupiedTask && IsValid(prevOccupiedTask))
			{
				prevOccupiedTask->RollBack();
			}
		}
		else if (Result == ETaskExecutorResult::Complete)
		{
			/// 태스크가 성공했다! 다음 단계로 넘어간다!
			TObjectPtr<UMissionTaskBase> prevOccupiedTask = OccupiedTask;
			++Step;

			OnStepChanged.Broadcast(this, Step);

			if (SequentialTaskList.IsValidIndex(Step))
			{
				OccupiedTask = SequentialTaskList[Step];
				if (IsValid(prevOccupiedTask))
				{
					prevOccupiedTask->Complete();
				}
			}
			else
			{
				// 모든 태스크 완료 (혹은 더 이상 태스크가 없음)
				SetState(EMissionExecutorState::Completed);

				OnCompleted.Broadcast(this);

                // UManager_TaskExecutor에 완료를 알립니다.
                if (UManager_Mission* TaskManager = UManager_Mission::Get())
                {
                    TaskManager->NotifyTaskExecutorCompletion(this); // 현재 UMissionTaskExecutor 인스턴스를 전달
                }
				return;
			}
		}
	}
}

TObjectPtr<UMissionTaskBase> UMissionTaskExecutor::CreateTask(const FMissionTaskInfo& inTaskInfo)
{
	switch (inTaskInfo.Type)
	{
	case EInGameTaskType::MISSION:
		return OnCreateTask_MISSION(inTaskInfo);
	case EInGameTaskType::SPAWN_OBJECT:
		return OnCreateTask_SPAWN_OBJECT(inTaskInfo);
	case EInGameTaskType::SPAWN_MONSTER:
		return OnCreateTask_SPAWN_MONSTER(inTaskInfo);
	default:
		UE_LOG(LogTemp, Warning, TEXT("UMissionTaskExecutor::CreateTask - Unhandled EInGameTaskType: %d"), (int32)inTaskInfo.Type);
		return nullptr;
	}
}

TObjectPtr<UMissionTaskBase> UMissionTaskExecutor::OnCreateTask_SPAWN_OBJECT(const FMissionTaskInfo& inTaskInfo)
{
	TObjectPtr<UMissionTask_ObjectSpawn> NewTask = NewObject<UMissionTask_ObjectSpawn>(this); // UMissionTask_Spawn을 재활용
	NewTask->SetData(inTaskInfo);
	return NewTask;
}

TObjectPtr<UMissionTaskBase> UMissionTaskExecutor::OnCreateTask_SPAWN_MONSTER(const FMissionTaskInfo& inTaskInfo)
{
	TObjectPtr<UMissionTask_Spawn> NewTask = NewObject<UMissionTask_Spawn>(this);
	NewTask->SetData(inTaskInfo);
	return NewTask;
}

TObjectPtr<UMissionTaskBase> UMissionTaskExecutor::OnCreateTask_MISSION(const FMissionTaskInfo& inTaskInfo)
{
	TObjectPtr<UMissionMultiTaskBase> NewTask = NewObject<UMissionMultiTaskBase>(this);
	NewTask->SetData(inTaskInfo);
	return NewTask;
}
#pragma endregion 