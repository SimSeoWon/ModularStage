#pragma once

#include "CoreMinimal.h"
#include "TableRowBase.h"
#include "TableEnum.h"
#include "TableHexGrid.generated.h"

/**
 * 개별 육각형 타일의 정보를 담는 구조체
 * 기존 Project_Alpha의 구조를 ModularStage에 맞게 확장함
 */
USTRUCT(BlueprintType)
struct MODULARSTAGE_API FHexagonTile
{
	GENERATED_BODY()

public:
	FHexagonTile() {}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile")
	int32 Index = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile")
	EHexagonTileType TileType = EHexagonTileType::NONE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile")
	EHexagonTileHeight HeightType = EHexagonTileHeight::NONE;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile")
	FVector VecCenter = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile")
	TArray<FVector> CornerList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Tile")
	int32 Group = -1;

	// --- ModularStage 확장 필드 ---

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mission")
	FString PrefabAssetPath; // 타일에 배치될 프리팹 에셋 경로

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mission")
	EMissionType MissionType = EMissionType::Main;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mission")
	int32 ExecutionOrder = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mission")
	int32 SubLevelID = 0;
};

/**
 * 육각형 타일맵 전체의 메타데이터 및 타일 리스트를 관리하는 데이터 테이블 구조체
 */
USTRUCT(BlueprintType)
struct MODULARSTAGE_API FRES_HEXAGONTILEMAP : public FTableRowStuffInt32
{
	GENERATED_BODY()

public:
	FRES_HEXAGONTILEMAP() {}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map")
	float TileSize = 100.0f; // 타일 크기 (반지름)

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map")
	int32 Column = 0; // 타일맵의 열(Column) 개수

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map")
	FVector Offset = FVector::ZeroVector; // 타일맵 배치 시작점 오프셋

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map")
	FVector Minimum = FVector::ZeroVector; // 전체 타일 영역의 최소 경계 좌표

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map")
	FVector Maximum = FVector::ZeroVector; // 전체 타일 영역의 최대 경계 좌표

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Map")
	TArray<FHexagonTile> TileList; // 타일 데이터 리스트
};
