#pragma once

#include "CoreMinimal.h"
#include "MissionTaskBase.h"
#include "MissionTask_SetCamera.generated.h"

UCLASS()
class MODULARSTAGE_API UMissionTask_SetCamera : public UMissionTaskBase
{
	GENERATED_BODY()

public:
	UMissionTask_SetCamera();
	~UMissionTask_SetCamera() {}

	virtual void Clear() override;
	virtual void SetData(const FMissionTaskInfo& InTaskInfo) override;
	virtual ETaskExecutorResult DoAction() override;

public:
	UPROPERTY()
	int32 TileIndex = -1;
};

