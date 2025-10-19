#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"

class FMissionTaskBase;
struct FMissionTaskInfo;

class MODULARSTAGE_API FMissionTaskExecutor
{
public:
    FMissionTaskExecutor();

    void Initialize(const FDataTableRowHandle& InMissionData);
    void Tick(float DeltaTime);
    bool IsInitialized() const { return bIsInitialized; }

protected:
#pragma region Create Task Function
    bool CreateTask(TSharedRef<FMissionTaskInfo> inTaskInfo);

    UFUNCTION()
    bool OnCreateTask_SPAWN(TSharedRef<FMissionTaskInfo> inTaskInfo);

    UFUNCTION()
    bool OnCreateTask_SPAWN_MONSTER(TSharedRef<FMissionTaskInfo> inTaskInfo);

    UFUNCTION()
    bool OnCreateTask_MARCH_ON(TSharedRef<FMissionTaskInfo> inTaskInfo);

    UFUNCTION()
    bool OnCreateTask_MISSION(TSharedRef<FMissionTaskInfo> inTaskInfo);
#pragma endregion

private:
    FDataTableRowHandle MissionData;
    bool bIsInitialized;
    // Add other state variables as needed

    int32 Step = 0;
    TArray<TSharedPtr<FMissionTaskBase>> SequentialTaskList;
    TSharedPtr<FMissionTaskBase> OccupiedTask;
};