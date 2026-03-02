#include "ModularStage_HexTileDataBase.h"
#include "ModularStage_HexTileView.h"

UModularStage_HexTileDataBase::UModularStage_HexTileDataBase()
{
	AppendCoordinates =
	{
		{FVector2D(-1, -1), FVector2D(-1, 0), FVector2D(-1, 1), FVector2D(0, 1), FVector2D(1, 0), FVector2D(0, -1)}, // 짝수 행
		{FVector2D(0, -1), FVector2D(-1, 0), FVector2D(0, 1), FVector2D(1, 1), FVector2D(1, 0), FVector2D(1, -1)},  // 홀수 행
	};
}

void UModularStage_HexTileDataBase::Init(FVector2D InCoordinates, float InSize, float InWidth, float InHeight, float InScale)
{
	Coordinates = InCoordinates;
	Size = InSize;
	Width = InWidth;
	Height = InHeight;
	Scale = InScale;

	float HalfWidth = Width / 2;
	float HalfHeight = Size;
	float QuarterHeight = HalfHeight / 2;

	// Staggered Grid 배치 로직 (Alpha_HexagonTileDataBase 반영)
	Center.X = Coordinates.X * Width + HalfWidth;
	Center.Y = Coordinates.Y * (Height - QuarterHeight) + HalfHeight;
	
	if (int32(Coordinates.Y) & 1)
	{
		Center.X += HalfWidth;
	}

	// 꼭짓점 좌표 계산 (Pointy Top, 충돌 검사용)
	CornerList.Reset();
	CornerList.Add(FVector2D(Center.X, Center.Y - HalfHeight));             // Top
	CornerList.Add(FVector2D(Center.X - HalfWidth, Center.Y - QuarterHeight)); // Top Left (시계 반대 방향 또는 특정 순서 유지)
	CornerList.Add(FVector2D(Center.X - HalfWidth, Center.Y + QuarterHeight)); // Bottom Left
	CornerList.Add(FVector2D(Center.X, Center.Y + HalfHeight));             // Bottom
	CornerList.Add(FVector2D(Center.X + HalfWidth, Center.Y + QuarterHeight)); // Bottom Right
	CornerList.Add(FVector2D(Center.X + HalfWidth, Center.Y - QuarterHeight)); // Top Right
}

FVector2D UModularStage_HexTileDataBase::GetAxialCoordinates()
{
	return OddToAxial(Coordinates);
}

void UModularStage_HexTileDataBase::GetAroundDoordinatesList(TArray<FVector2D>& OutCoordinatesList)
{
	int32 Parity = int32(Coordinates.Y) & 1;

	OutCoordinatesList =
	{
		AddOffset(Coordinates, AppendCoordinates[Parity][0]),
		AddOffset(Coordinates, AppendCoordinates[Parity][1]),
		AddOffset(Coordinates, AppendCoordinates[Parity][2]),
		AddOffset(Coordinates, AppendCoordinates[Parity][3]),
		AddOffset(Coordinates, AppendCoordinates[Parity][4]),
		AddOffset(Coordinates, AppendCoordinates[Parity][5]),
	};
}

FVector2D UModularStage_HexTileDataBase::GetTopPostion()
{
	if (CornerList.Num() > 0) return CornerList[0];
	return FVector2D::ZeroVector;
}

bool UModularStage_HexTileDataBase::CollisionCheck(const FVector2D InPos)
{
	if (TriangleCollisionCheck(Center, CornerList[0], CornerList[1], InPos)) return true;
	if (TriangleCollisionCheck(Center, CornerList[1], CornerList[2], InPos)) return true;
	if (TriangleCollisionCheck(Center, CornerList[2], CornerList[3], InPos)) return true;
	if (TriangleCollisionCheck(Center, CornerList[3], CornerList[4], InPos)) return true;
	if (TriangleCollisionCheck(Center, CornerList[4], CornerList[5], InPos)) return true;
	if (TriangleCollisionCheck(Center, CornerList[5], CornerList[0], InPos)) return true;

	return false;
}

bool UModularStage_HexTileDataBase::TriangleCollisionCheck(FVector2D PosA, FVector2D PosB, FVector2D PosC, const FVector2D InPos)
{
	FVector VecAB = FVector(PosB.X - PosA.X, PosB.Y - PosA.Y, 0);
	FVector VecAC = FVector(PosC.X - PosA.X, PosC.Y - PosA.Y, 0);
	FVector VecAP = FVector(InPos.X - PosA.X, InPos.Y - PosA.Y, 0);

	FVector VecBC = FVector(PosC.X - PosB.X, PosC.Y - PosB.Y, 0);
	FVector VecBP = FVector(InPos.X - PosB.X, InPos.Y - PosB.Y, 0);

	FVector VecCA = FVector(PosA.X - PosC.X, PosA.Y - PosC.Y, 0);
	FVector VecCP = FVector(InPos.X - PosC.X, InPos.Y - PosC.Y, 0);

	FVector Normal = FVector::CrossProduct(VecAB, VecAC);
	Normal.Normalize();

	float Dot1 = FVector::DotProduct(Normal, FVector::CrossProduct(VecAB, VecAP));
	float Dot2 = FVector::DotProduct(Normal, FVector::CrossProduct(VecBC, VecBP));
	float Dot3 = FVector::DotProduct(Normal, FVector::CrossProduct(VecCA, VecCP));

	if (Dot1 >= 0 && Dot2 >= 0 && Dot3 >= 0) return true;
	if (Dot1 <= 0 && Dot2 <= 0 && Dot3 <= 0) return true;

	return false;
}

void UModularStage_HexTileDataBase::Refresh()
{
	if (PtrTileObject.IsValid())
	{
		PtrTileObject->Refresh();
	}
}

void UModularStage_HexTileDataBase::SetTileObject(UModularStage_HexTileItem* InTileObjectPtr)
{
	PtrTileObject = InTileObjectPtr;
}

void UModularStage_HexTileDataBase::SetTileType(EHexagonTileType InTileType) 
{ 
	TileType = InTileType; 
	Refresh();
}

void UModularStage_HexTileDataBase::SetHeightType(EHexagonTileHeight InHeightType) 
{ 
	HeightType = InHeightType; 
	Refresh();
}

void UModularStage_HexTileDataBase::SetTileType(FName InTypeName)
{
	const UEnum* PtrEnum = StaticEnum<EHexagonTileType>();
	if (PtrEnum) TileType = (EHexagonTileType)PtrEnum->GetValueByName(InTypeName);
}

void UModularStage_HexTileDataBase::SetHeightType(FName InTypeName)
{
	const UEnum* PtrEnum = StaticEnum<EHexagonTileHeight>();
	if (PtrEnum) HeightType = (EHexagonTileHeight)PtrEnum->GetValueByName(InTypeName);
}

FVector2D UModularStage_HexTileDataBase::CubeToAxial(FVector Cube) { return FVector2D(Cube.X, Cube.Y); }
FVector UModularStage_HexTileDataBase::AxialToCube(FVector2D Hex) { return FVector(Hex.X, Hex.Y, -Hex.X - Hex.Y); }

FVector2D UModularStage_HexTileDataBase::OddToAxial(FVector2D InOffset)
{
	int32 Q = (int32)InOffset.X - ((int32)InOffset.Y - ((int32)InOffset.Y & 1)) / 2;
	int32 R = (int32)InOffset.Y;
	return FVector2D(Q, R);
}

FVector2D UModularStage_HexTileDataBase::AddOffset(FVector2D A, FVector2D B) { return A + B; }
