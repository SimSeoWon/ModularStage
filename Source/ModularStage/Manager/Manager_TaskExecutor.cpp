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
    // ��� �ε�� UClass�� ��ȸ�մϴ�.
    for (TObjectIterator<UClass> It; It; ++It)
    {
        // �� Ŭ����(UCommonUserSubsystem)�� �ڽ� Ŭ�����̸鼭,
        // �ڱ� �ڽ��� �ƴ� �ٸ� Ŭ������ ã���ϴ�.
        if (It->IsChildOf(UManager_TaskExecutor::StaticClass()) && (*It != UManager_TaskExecutor::StaticClass()))
        {
            // ����ڰ� ���� �Ļ� Ŭ������ �����ϹǷ�,
            // �� �⺻ Ŭ������ �ν��Ͻ��� �����ϸ� �� �˴ϴ�.
            return false;
        }
    }

    // ������ ���ƴµ��� �ٸ� �ڽ� Ŭ������ ã�� ���ߴٸ�,
    // ����ڴ� �� �⺻ Ŭ������ ����Ϸ��� ���̹Ƿ� �ν��Ͻ��� �����ؾ� �մϴ�.
    return true;
}

