#include "Manager_TaskExecutor.h"
#include "Subsystems/SubsystemCollection.h"
#include "UObject/UObjectIterator.h"

#include "ModularStage/Mission/MissionTaskExecutor.h"

UManager_TaskExecutor* UManager_TaskExecutor::Get() 
{
    return GetManager<UManager_TaskExecutor>();
}

void UManager_TaskExecutor::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UManager_TaskExecutor::Deinitialize()
{
	Super::Deinitialize();
}

bool UManager_TaskExecutor::ShouldCreateSubsystem(UObject* Outer) const
{
    // 모든 로드된 UClass를 순회합니다.
    for (TObjectIterator<UClass> It; It; ++It)
    {
        // 이 클래스(UCommonUserSubsystem)의 자식 클래스이면서,
        // 자기 자신은 아닌 다른 클래스를 찾습니다.
        if (It->IsChildOf(UManager_TaskExecutor::StaticClass()) && (*It != UManager_TaskExecutor::StaticClass()))
        {
            // 사용자가 만든 파생 클래스가 존재하므로,
            // 이 기본 클래스는 인스턴스를 생성하면 안 됩니다.
            return false;
        }
    }

    // 루프를 마쳤는데도 다른 자식 클래스를 찾지 못했다면,
    // 사용자는 이 기본 클래스를 사용하려는 것이므로 인스턴스를 생성해야 합니다.
    return true;
}

