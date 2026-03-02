#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ModularStage/Table/TableHexGrid.h"
#include "ModularStage_HexTileDataBase.generated.h"

class UModularStage_HexTileItem; // 위젯 클래스 (추후 생성)

/**
 * 에디터 및 런타임 UI에서 육각형 타일의 개별 데이터를 관리하는 클래스
 */
UCLASS(BlueprintType)
class MODULARSTAGE_API UModularStage_HexTileDataBase : public UObject
{
	GENERATED_BODY()

public:
	UModularStage_HexTileDataBase();

	void Init(FVector2D InCoordinates, float InSize, float InWidth, float InHeight, float InScale);

	// --- 좌표 변환 및 연산 ---
	FVector2D GetAxialCoordinates(); // 배열 좌표(Offset)를 축 좌표(Axial)로 변환
	void GetAroundDoordinatesList(TArray<FVector2D>& OutCoordinatesList); // 인접 타일 좌표 목록
	FVector2D GetTopPostion(); // 타일의 상단 꼭짓점 위치
	bool CollisionCheck(const FVector2D InPos); // 특정 평면 좌표가 타일 내부에 있는지 검사

	// --- 게터/세터 ---
	void SetIsSelect(bool bInSelect) { bIsSelect = bInSelect; }
	bool GetIsSelect() const { return bIsSelect; }
	
	void SetTileObject(UModularStage_HexTileItem* InTileObjectPtr);
	void Refresh(); // 위젯 업데이트 요청

	void SetTileType(EHexagonTileType InTileType);
	void SetHeightType(EHexagonTileHeight InHeightType);
	void SetTileType(FName InTypeName);
	void SetHeightType(FName InTypeName);

private:
	bool TriangleCollisionCheck(FVector2D PosA, FVector2D PosB, FVector2D PosC, const FVector2D InPos);
	FVector2D CubeToAxial(FVector Cube);
	FVector AxialToCube(FVector2D Hex);
	FVector2D OddToAxial(FVector2D Offset);
	FVector2D AddOffset(FVector2D A, FVector2D B);

public:
	// --- 타일 기본 정보 ---
	UPROPERTY(BlueprintReadWrite, Category = "HexTile")
	EHexagonTileType TileType = EHexagonTileType::NONE;

	UPROPERTY(BlueprintReadWrite, Category = "HexTile")
	EHexagonTileHeight HeightType = EHexagonTileHeight::NONE;

	UPROPERTY(BlueprintReadWrite, Category = "HexTile")
	int32 Index = 0;

	UPROPERTY(BlueprintReadWrite, Category = "HexTile")
	int32 GroupID = -1;

	UPROPERTY(BlueprintReadWrite, Category = "HexTile")
	FVector2D Coordinates; // 그리드 배열 좌표 (X=Col, Y=Row)

	UPROPERTY(BlueprintReadWrite, Category = "HexTile")
	FVector2D Center; // 로컬 중심 좌표

	UPROPERTY(BlueprintReadWrite, Category = "HexTile")
	float Size = 0.0f; // 반지름

	UPROPERTY(BlueprintReadWrite, Category = "HexTile")
	float Scale = 1.0f;

	UPROPERTY(BlueprintReadWrite, Category = "HexTile")
	float Height = 0.0f;

	UPROPERTY(BlueprintReadWrite, Category = "HexTile")
	float Width = 0.0f;

	UPROPERTY(BlueprintReadWrite, Category = "HexTile")
	TArray<FVector2D> CornerList; // 6개의 꼭짓점 좌표

	// --- ModularStage 확장 필드 (미션 및 프리팹) ---
	UPROPERTY(BlueprintReadWrite, Category = "HexTile|Mission")
	FString PrefabAssetPath;

	UPROPERTY(BlueprintReadWrite, Category = "HexTile|Mission")
	EMissionType MissionType = EMissionType::Main;

	UPROPERTY(BlueprintReadWrite, Category = "HexTile|Mission")
	int32 ExecutionOrder = 0;

	UPROPERTY(BlueprintReadWrite, Category = "HexTile|Mission")
	int32 SubLevelID = 0;

private:
	bool bIsSelect = false;

	TArray<TArray<FVector2D>> AppendCoordinates; // 인접 좌표 오프셋 (짝수/홀수 행 구분)

	TWeakObjectPtr<UModularStage_HexTileItem> PtrTileObject;
};
