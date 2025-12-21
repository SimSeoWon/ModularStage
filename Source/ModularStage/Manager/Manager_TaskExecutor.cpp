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
    //UClass
    for (TObjectIterator<UClass> It; It; ++It)
    {
        if (It->IsChildOf(UManager_TaskExecutor::StaticClass()) && (*It != UManager_TaskExecutor::StaticClass()))
        {

            return false;
        }
    }

    return true;
}

