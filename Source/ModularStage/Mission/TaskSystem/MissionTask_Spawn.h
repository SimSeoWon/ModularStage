#pragma once

#include "CoreMinimal.h"
#include "MissionTaskBase.h"
#include "ModularStage/Table/TableEnum.h" // For EInGameSpawnType
#include "MissionTask_Spawn.generated.h"

UCLASS()
class MODULARSTAGE_API UMissionTask_Spawn : public UMissionTaskBase
{
	GENERATED_BODY()

public:
	UMissionTask_Spawn();
	~UMissionTask_Spawn() {}

	virtual void Clear() override;
	virtual void SetData(const FMissionTaskInfo& InTaskInfo) override;
	virtual ETaskExecutorResult DoAction() override;

public:
	UPROPERTY()
	uint64 ActorSN = 0;
	UPROPERTY()
	int32 ActorID = 0;
	UPROPERTY()
	int32 TableID = 0;
	
	UPROPERTY()
	EInGameSpawnType SpawnType;
	UPROPERTY()
	int32 TileIndex = -1;
};

