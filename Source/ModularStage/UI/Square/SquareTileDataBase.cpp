// Fill out your copyright notice in the Description page of Project Settings.


#include "SquareTileDataBase.h"
#include "SquareTileItem.h"


void USquareTileDataBase::Init(FVector2D inCoordinates, float inSize, float inScale)
{
	//좌표
	Coordinates.X = inCoordinates.X;
	Coordinates.Y = inCoordinates.Y;

	// 사이즈
	Scale = inScale;
	Size = inSize;

	float halfSize = Size / 2;

	//중심점
	Center.X = (Coordinates.X * Size) + halfSize;
	Center.Y = (Coordinates.Y * Size) + halfSize;

	// 꼭지점
	CornerList =
	{
		FVector2D(Center.X - halfSize, Center.Y - halfSize), // Top-Left
		FVector2D(Center.X + halfSize, Center.Y - halfSize), // Top-Right
		FVector2D(Center.X + halfSize, Center.Y + halfSize), // Bottom-Right
		FVector2D(Center.X - halfSize, Center.Y + halfSize), // Bottom-Left
	};

}

bool USquareTileDataBase::CollisionCheck(const FVector2D inPos)
{
	float halfSize = Size / 2;

	if (inPos.X >= Center.X - halfSize && inPos.X <= Center.X + halfSize &&
		inPos.Y >= Center.Y - halfSize && inPos.Y <= Center.Y + halfSize)
	{
		return true;
	}

	return false;
}

void USquareTileDataBase::Refresh()
{
	if (PtrTileObject.IsValid() == false)
		return;

	PtrTileObject->Refresh();
}

void USquareTileDataBase::SetTileObject(USquareTileItem* inTileObjectPtr)
{
	PtrTileObject = inTileObjectPtr;
}