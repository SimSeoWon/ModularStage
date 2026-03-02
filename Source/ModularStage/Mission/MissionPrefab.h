// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Table/MissionTaskTable.h" // Required for FMissionTaskInfo
#include "ModularStage/System/HexGridLibrary.h" // Required for FHexCoord
#include "MissionPrefab.generated.h"

class UActorComponent_MissionTaskExecutor;
class UStaticMeshComponent;
class UTaskListEditorWidget;
class UHexGridVisualComponent;

#if WITH_EDITOR
class UEntryData_InGameTaskList;
#endif

/** 육각형 타일의 개별 역할 정의 */
UENUM(BlueprintType)
enum class EHexTileRole : uint8
{
	None = 0,		// 비어있거나 무효한 타일
	Floor,			// 일반적인 바닥
	Wall,			// 지나갈 수 없는 벽/장애물
	Entry,			// 미션 진입점
	Exit,			// 미션 완료 후 다음 맵으로 연결되는 통로
	Event			// 특정 이벤트(NPC, 보물 상자 등) 발생 지점
};

/** 개별 타일의 상세 설정 */
USTRUCT(BlueprintType)
struct FHexTileConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hex")
	FHexCoord Coord; // 상대 좌표 (0,0 이 프리팹의 중심)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hex")
	EHexTileRole Role = EHexTileRole::Floor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hex")
	bool bIsActive = true; // 실제 맵 생성 시 이 타일을 사용할 것인가?

	/** [단일화] 타일의 실제 중앙 위치 좌표 (프리팹 기준 상대 좌표) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Hex")
	FVector CenterLocation = FVector::ZeroVector;

	FHexTileConfig() {}
	FHexTileConfig(FHexCoord InCoord, EHexTileRole InRole = EHexTileRole::Floor, FVector InLocation = FVector::ZeroVector)
		: Coord(InCoord), Role(InRole), CenterLocation(InLocation) {}
};

/**
 * @brief 미션의 기본 단위가 되는 프리팹입니다.
 * 7개의 육각형 타일(Septet Cluster)로 구성된 하나의 모듈 단위입니다.
 */
UCLASS()
class MODULARSTAGE_API AMissionPrefab : public AActor
{
	GENERATED_BODY()

	friend UTaskListEditorWidget;

public:
	// Sets default values for this actor's properties
	AMissionPrefab();

	virtual void PostInitProperties() override;

	/** 에디터에서 이동 시 자동으로 육각형 그리드 중심에 맞춥니다. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mission | HexGrid")
	bool bSnapToHexGrid = true;

	/** 실제 게임 중에도 육각형 그리드 가이드를 표시할지 여부입니다. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mission | HexGrid")
	bool bShowHexGridInGame = false;

	/** 디테일 패널에서 이 프리팹을 개별 액터들로 분해하여 레벨에 배치합니다. */
	UFUNCTION(CallInEditor, Category = "Prefab")
	void UnpackForEditing();

	/** Opens the Mesh Generator utility widget to create a custom mesh for this prefab. */
	UFUNCTION(CallInEditor, Category = "Prefab | Mesh Generation")
	void OpenMeshGenerator();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** 에디터 상에서 7개 타일의 가이드라인과 현재 설정을 시각화합니다. */
	virtual void OnConstruction(const FTransform& Transform) override;

#if WITH_EDITOR
	void SetTaskList(const  TArray<UEntryData_InGameTaskList*>& inEntryDataList);
#endif

	/** 특정 타일 인덱스(0~6)의 상대 월드 좌표를 반환합니다. */
	UFUNCTION(BlueprintPure, Category = "Mission | HexGrid")
	FVector GetTileLocation(int32 Index) const;

protected:
	/** 7개의 타일 클러스터 데이터 (고정 크기 7) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mission | HexGrid")
	TArray<FHexTileConfig> SeptetTiles;

	/** 에디터 시각화를 위한 전용 컴포넌트 */
	UPROPERTY()
	TObjectPtr<UHexGridVisualComponent> HexGridVisualizer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mission")
	TObjectPtr<UActorComponent_MissionTaskExecutor> MissionTaskExecutor;

	/** The list of mission tasks associated with this prefab. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mission")
	TArray<FMissionTaskInfo> TaskList;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	/** Defines the size of the simple box obstacle to generate. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prefab | Mesh Generation")
	FVector ObstacleExtent;
};
