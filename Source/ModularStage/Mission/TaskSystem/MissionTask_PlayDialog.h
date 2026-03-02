#pragma once

#include "CoreMinimal.h"
#include "MissionTaskBase.h"
#include "MissionTask_PlayDialog.generated.h"

UCLASS()
class MODULARSTAGE_API UMissionTask_PlayDialog : public UMissionTaskBase
{
	GENERATED_BODY()

public:
	UMissionTask_PlayDialog();
	~UMissionTask_PlayDialog() {}

	virtual void Clear() override;
	virtual void SetData(const FMissionTaskInfo& InTaskInfo) override;
	virtual ETaskExecutorResult DoAction() override;

public:
	UPROPERTY()
	int32 TileIndex = -1;
	UPROPERTY()
	int32 DialogID = 0;
};
