// Fill out your copyright notice in the Description page of Project Settings.


#include "Alpha_HexagonTileDataBase.h"
#include "Alpha_HexagonTileItem.h"
#include "ModularStage/Table/TableEnum.h"


void UAlpha_HexagonTileDataBase::Init(FVector2D inCoordinates, float inSize, float inWidth, float inHeight, float inScale)
{
	HeightType = EHexagonTileHeight::SeaLevel;

	//��ǥ
	Coordinates.X = inCoordinates.X;
	Coordinates.Y = inCoordinates.Y;

	// ������
	Scale = inScale;
	Size = inSize;
	Width = inWidth;
	Height = inHeight;


	float halfHeight = Size;
	float quarterHeight = halfHeight / 2;
	float halfWidth = Width / 2;

	//������
	Center.X = (Coordinates.X * Width) + halfWidth;
	Center.Y = (Coordinates.Y * (Height - quarterHeight)) + halfHeight;
	if (int32(Coordinates.Y) % 2 != 0) // Ȧ���� ������ �о��ش�.
		Center.X += halfWidth;

	// �ϴ� �浹 �˻縦 ���ؼ� �ݽð� �������� �־����.

	CornerList =
	{
		FVector2D(Center.X, Center.Y - halfHeight), // �ֻ��
		FVector2D(Center.X - halfWidth, Center.Y - quarterHeight), // �� ���
		FVector2D(Center.X - halfWidth, Center.Y + quarterHeight), // �� �ϴ�
		FVector2D(Center.X, Center.Y + halfHeight), // ���ϴ�
		FVector2D(Center.X + halfWidth, Center.Y + quarterHeight), // �� �ϴ�
		FVector2D(Center.X + halfWidth, Center.Y - quarterHeight), // �� ���
	};

	AppendCoordinates =
	{
		// ¦����
		{FVector2D(-1, -1), FVector2D(-1, 0), FVector2D(-1, 1), FVector2D(0, 1), FVector2D(1, 0), FVector2D(0, -1)},
		// Ȧ����
		{FVector2D(0, -1), FVector2D(-1, 0), FVector2D(0, 1), FVector2D(1, 1), FVector2D(1, 0), FVector2D(1, -1)},
	};

}

bool UAlpha_HexagonTileDataBase::CollisionCheck(const FVector2D inPos)
{
	//6���� �ﰢ������ �����ؼ� üũ����
	if (CollisionCheck(Center, CornerList[0], CornerList[1], inPos))
		return true;

	if (CollisionCheck(Center, CornerList[1], CornerList[2], inPos))
		return true;

	if (CollisionCheck(Center, CornerList[2], CornerList[3], inPos))
		return true;

	if (CollisionCheck(Center, CornerList[3], CornerList[4], inPos))
		return true;

	if (CollisionCheck(Center, CornerList[4], CornerList[5], inPos))
		return true;

	if (CollisionCheck(Center, CornerList[5], CornerList[0], inPos))
		return true;

	return false;

}

bool UAlpha_HexagonTileDataBase::CollisionCheck(FVector2D posA, FVector2D posB, FVector2D posC, const FVector2D inPos)
{
	FVector vectorAB = FVector(posB.X, posB.Y, 0) - FVector(posA.X, posA.Y, 0);
	FVector vectorAC = FVector(posC.X, posC.Y, 0) - FVector(posA.X, posA.Y, 0);

	FVector vectorBC = FVector(posC.X, posC.Y, 0) - FVector(posB.X, posB.Y, 0);
	FVector vectorCA = FVector(posA.X, posA.Y, 0) - FVector(posC.X, posC.Y, 0);

	FVector vectorAP = FVector(inPos.X, inPos.Y, 0) - FVector(posA.X, posA.Y, 0);
	FVector vectorBP = FVector(inPos.X, inPos.Y, 0) - FVector(posB.X, posB.Y, 0);
	FVector vectorCP = FVector(inPos.X, inPos.Y, 0) - FVector(posC.X, posC.Y, 0);

	FVector vecNormal = FVector::CrossProduct(vectorAB, vectorAC);
	vecNormal.Normalize();

	FVector vacCross_AB_AP = FVector::CrossProduct(vectorAB, vectorAP);
	FVector vacCross_BC_BP = FVector::CrossProduct(vectorBC, vectorBP);
	FVector vacCross_CA_CP = FVector::CrossProduct(vectorCA, vectorCP);

	// ����
	if (FVector::DotProduct(vecNormal, vacCross_AB_AP) <= 0)
		return false;

	if (FVector::DotProduct(vecNormal, vacCross_BC_BP) <= 0)
		return false;

	if (FVector::DotProduct(vecNormal, vacCross_CA_CP) <= 0)
		return false;

	return true;

}

FVector2D UAlpha_HexagonTileDataBase::GetTopPostion()
{
	return CornerList[0];
}

FVector UAlpha_HexagonTileDataBase::AxialToCube(FVector2D hex)
{
	int32 q = hex.X;
	int32 r = hex.Y;
	int32 s = -q - r;
	return FVector(q, r, s);
}

FVector2D UAlpha_HexagonTileDataBase::CubeToAxial(FVector cube)
{
	int32 q = cube.X;
	int32 r = cube.Y;

	return FVector2D(q, r);
}

FVector2D UAlpha_HexagonTileDataBase::OddToAxial(FVector2D offset)
{
	int32 col = offset.X;
	int32 row = offset.Y;

	int32 r = row;
	int32 q = col - (row - (row & 0x0001)) / 2;

	return FVector2D(q, r);
}

FVector2D UAlpha_HexagonTileDataBase::GetAxialCoordinates()
{
	return OddToAxial(Coordinates);
}

FVector2D UAlpha_HexagonTileDataBase::AddOffset(FVector2D a, FVector2D b)
{
	return FVector2D(a.X + b.X, a.Y + b.Y);
}

void UAlpha_HexagonTileDataBase::GetAroundDoordinatesList(TArray<FVector2D>& outCoordinatesList)
{
	int32 parity = int32(Coordinates.Y) & 1;

	outCoordinatesList =
	{
		AddOffset(Coordinates, AppendCoordinates[parity][0]),
		AddOffset(Coordinates, AppendCoordinates[parity][1]),
		AddOffset(Coordinates, AppendCoordinates[parity][2]),
		AddOffset(Coordinates, AppendCoordinates[parity][3]),
		AddOffset(Coordinates, AppendCoordinates[parity][4]),
		AddOffset(Coordinates, AppendCoordinates[parity][5]),
	};
}

void UAlpha_HexagonTileDataBase::Refresh()
{
	if (PtrTileObject.IsValid() == false)
		return;

	PtrTileObject->Refresh();
}

void UAlpha_HexagonTileDataBase::SetTileObject(UAlpha_HexagonTileItem* inTileObjectPtr)
{
	PtrTileObject = inTileObjectPtr;
}

void UAlpha_HexagonTileDataBase::SetTileType(EHexagonTileType inTileType)
{
	if (EHexagonTileType::NONE == inTileType)
		inTileType = EHexagonTileType::Water;

	TileType = inTileType;

	Refresh();
}

void UAlpha_HexagonTileDataBase::SetHeightType(EHexagonTileHeight inHeightType) 
{
	if (EHexagonTileHeight::NONE == inHeightType)
		inHeightType = EHexagonTileHeight::SeaLevel;

	HeightType = inHeightType;

	Refresh();
}

void UAlpha_HexagonTileDataBase::SetTileType(FName inTypeName)
{
	const UEnum* ptrEnum = FindObject<UEnum>(ANY_PACKAGE, TEXT("EHexagonTileType"), true);
	if (nullptr == ptrEnum)
		return;

	EHexagonTileType type = StaticCast<EHexagonTileType>(ptrEnum->GetValueByName(inTypeName));
	if(EHexagonTileType::NONE == type)
		return;

	/**UI�� ��������!*/
	SetTileType(type);
}

void UAlpha_HexagonTileDataBase::SetHeightType(FName inTypeName)
{
	const UEnum* ptrEnum = FindObject<UEnum>(ANY_PACKAGE, TEXT("EHexagonTileHeight"), true);
	if (nullptr == ptrEnum)
		return;

	EHexagonTileHeight type = StaticCast<EHexagonTileHeight>(ptrEnum->GetValueByName(inTypeName));
	if (EHexagonTileHeight::NONE == type)
		return;

	/**UI�� ��������!*/
	SetHeightType(type);
	
}