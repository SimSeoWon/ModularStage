#pragma once

#include "CoreMinimal.h"
#include "InstancedStruct.h"
#include "ModularStage/Table/TableEnum.h"
#include "MissionTaskData.generated.h"

/**
 * 모든 미션 태스크 상세 데이터의 부모 구조체입니다.
 */
USTRUCT(BlueprintType)
struct MODULARSTAGE_API FMissionTaskData_Base
{
	GENERATED_BODY()

	virtual ~FMissionTaskData_Base() {}
};

/**
 * [Spawn Monster] 태스크를 위한 상세 데이터입니다.
 */
USTRUCT(BlueprintType)
struct MODULARSTAGE_API FMissionTaskData_Monster : public FMissionTaskData_Base
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Monster")
	int32 MonsterID = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	int32 TileIndex = -1;
};

/**
 * [Spawn Object] 태스크를 위한 상세 데이터입니다.
 */
USTRUCT(BlueprintType)
struct MODULARSTAGE_API FMissionTaskData_Object : public FMissionTaskData_Base
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Object")
	int32 ActorID = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	int32 TileIndex = -1;
};

/**
 * [MISSION] 태스크를 위한 상세 데이터입니다.
 * 하위에 디펜스, 호위 등 목표 타입을 포함합니다.
 */
USTRUCT(BlueprintType)
struct MODULARSTAGE_API FMissionTaskData_Mission : public FMissionTaskData_Base
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mission")
	EMissionObjectiveType ObjectiveType = EMissionObjectiveType::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mission")
	int32 TargetID = 0; // 목표 대상 ID (예: 호위 대상, 방어 지점 등)
};

/**
 * [Future Extensions] 시퀀스, 다이얼로그 등을 위한 예약 구조체들 (나중에 구현)
 */
USTRUCT(BlueprintType)
struct MODULARSTAGE_API FMissionTaskData_Sequence : public FMissionTaskData_Base
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sequence")
	int32 SequenceID = 0;
};

USTRUCT(BlueprintType)
struct MODULARSTAGE_API FMissionTaskData_Dialog : public FMissionTaskData_Base
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialog")
	int32 DialogID = 0;
};
