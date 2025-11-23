// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SquareTileDataBase.generated.h"

class USquareTileItem;
/**
 * 
 */
UCLASS()
class MODULARSTAGE_API USquareTileDataBase : public UObject
{
	GENERATED_BODY()
public:
	void Init(FVector2D inCoordinates, float inSize, float inScale);

	bool CollisionCheck(const FVector2D inPos); //  ٸü 浹 ˻.

	bool GetIsSelect() { return IsSelect; }
	void SetIsSelect(bool isSelect) { IsSelect = isSelect; }

	float GetSize() { return Size; }

	void Refresh();

	void SetTileObject(USquareTileItem* inTileObjectPtr);

	bool IsMovable() const { return bIsMovable; }
	void SetMovable(bool bNewMovable) { bIsMovable = bNewMovable; }

private:
public:
	int32 Index = 0;		// ε
	int32 GroupID = -1;		// ȹ 
	FVector2D Coordinates; //  ǥ
	FVector2D Center;  //  ġ
	float Size = 0.0f; // 
	float Scale = 100.0f; // ô
	int32 SubLevelID = 0;
	TArray<FVector2D> CornerList; //  4

	bool IsSelect = false;
	bool bIsMovable = true;

	TWeakObjectPtr<USquareTileItem> PtrTileObject;
};
