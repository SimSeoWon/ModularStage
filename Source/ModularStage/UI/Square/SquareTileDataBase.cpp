// Fill out your copyright notice in the Description page of Project Settings.


#include "SquareTileDataBase.h"
#include "SquareTileItem.h"


void USquareTileDataBase::Init(FVector2D inCoordinates, float inSize, float inWidth, float inHeight, float inScale)
{
	//좌표
	Coordinates.X = inCoordinates.X;
	Coordinates.Y = inCoordinates.Y;

	// 사이즈
	Scale = inScale;
	Size = inSize;
	Width = inWidth;
	Height = inHeight;


	float halfWidth = Width / 2;
	float halfHeight = Height / 2;

	//중심점
	Center.X = (Coordinates.X * Width) + halfWidth;
	Center.Y = (Coordinates.Y * Height) + halfHeight;

	// 꼭지점
	CornerList =
	{
		FVector2D(Center.X - halfWidth, Center.Y - halfHeight), // Top-Left
		FVector2D(Center.X + halfWidth, Center.Y - halfHeight), // Top-Right
		FVector2D(Center.X + halfWidth, Center.Y + halfHeight), // Bottom-Right
		FVector2D(Center.X - halfWidth, Center.Y + halfHeight), // Bottom-Left
	};

}

bool USquareTileDataBase::CollisionCheck(const FVector2D inPos)
{
	float halfWidth = Width / 2;
	float halfHeight = Height / 2;

	if (inPos.X >= Center.X - halfWidth && inPos.X <= Center.X + halfWidth &&
		inPos.Y >= Center.Y - halfHeight && inPos.Y <= Center.Y + halfHeight)
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