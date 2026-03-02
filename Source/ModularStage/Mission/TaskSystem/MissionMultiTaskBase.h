#pragma once

#include "CoreMinimal.h"
#include "MissionTaskBase.h"
#include "ModularStage/Table/MissionTaskTable.h"
#include "MissionMultiTaskBase.generated.h"

UCLASS()
class MODULARSTAGE_API UMissionMultiTaskBase : public UMissionTaskBase
{
	GENERATED_BODY()

public:
	UMissionMultiTaskBase();
	
	virtual void Clear() override;
	virtual void SetData(const FMissionTaskInfo& InTaskInfo) override;
	virtual ETaskExecutorResult DoAction() override;

	void AddSubTask(TObjectPtr<UMissionTaskBase> inSubTask);
	int32 GetStep() { return Step; }
	int32 GetTaskCount() { return SubTaskList.Num(); }
	TObjectPtr<UMissionTaskBase> GetTaskWrapper(int32 inIndex)
	{
		if (false == SubTaskList.IsValidIndex(inIndex))
			return nullptr;

		return SubTaskList[inIndex];
	}

protected:
	UPROPERTY()
	TArray<TObjectPtr<UMissionTaskBase>> SubTaskList;
	UPROPERTY()
	TObjectPtr<UMissionTaskBase> OccupiedTask;

	UPROPERTY()
	FMissionTaskInfo TaskInfo;
	
	UPROPERTY()
	int32 Step = 0;
	UPROPERTY()
	EInGameTaskType TaskType = EInGameTaskType::NONE;
	UPROPERTY()
	FString Title = TEXT("");
	UPROPERTY()
	FString Desc = TEXT("");
};
