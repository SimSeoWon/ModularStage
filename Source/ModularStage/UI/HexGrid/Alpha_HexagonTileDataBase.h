// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ModularStage/Table/TableEnum.h"
#include "Alpha_HexagonTileDataBase.generated.h"

class UAlpha_HexagonTileItem;

/**
 * 
 */
UCLASS()
class MODULARSTAGE_API UAlpha_HexagonTileDataBase : public UObject
{
	GENERATED_BODY()
public:
	void Init(FVector2D inCoordinates, float inSize, float inWidth, float inHeight, float inScale);

	FVector2D GetAxialCoordinates(); // 2�� ��ǥ��� �����Ѵ�.

	void GetAroundDoordinatesList(TArray<FVector2D>& outCoordinatesList);

	FVector2D GetTopPostion();
	bool CollisionCheck(const FVector2D inPos); // ���� �ٸ�ü �浹 �˻�.

	bool GetIsSelect() { return IsSelect; }
	void SetIsSelect(bool isSelect) { IsSelect = isSelect; }

	float GetSize() { return Size; }

	void Refresh();

	void SetTileObject(UAlpha_HexagonTileItem* inTileObjectPtr);

	void SetTileType(EHexagonTileType inTileType);
	void SetHeightType(EHexagonTileHeight inHeightType);

	void SetTileType(FName inTypeName);
	void SetHeightType(FName inTypeName);

private:
	bool CollisionCheck(FVector2D posA, FVector2D posB, FVector2D posC, const FVector2D inPos);

	FVector2D CubeToAxial(FVector cube); // 3�� ��ǥ�踦 2�� ��ǥ��� �����Ѵ�.
	FVector AxialToCube(FVector2D hex); // 2�� ��ǥ�踦 3�� ��ǥ��� �����Ѵ�.

	FVector2D OddToAxial(FVector2D offset); // �迭 ���¸� 2�� ��ǥ��� ����.

	FVector2D AddOffset(FVector2D a, FVector2D b);
	//FVector2D AxialToOddr(FVector2D hex); // 2�� ��ǥ�踦 �迭���·� ����... ���� ó���� �ȵȴ�...

public:
	EHexagonTileType TileType;
	EHexagonTileHeight HeightType;
	int32 Index = 0;		// �ε���
	int32 GroupID = -1;		// ��ȹ ����
	FVector2D Coordinates; //  ��ǥ
	FVector2D Center;  // ���� ��ġ
	float Size = 0.0f; // ������
	float Scale = 100.0f; // ��ô
	float Height = 0.0f; //  2 * size;
	float Width = 0.0f; // sqrt(3) * size;
	int32 SubLevelID = 0;
	TArray<FVector2D> CornerList; // ������ 6��

	TArray<TArray<FVector2D>> AppendCoordinates;

	bool IsSelect = false;

	TWeakObjectPtr<UAlpha_HexagonTileItem> PtrTileObject;
};
