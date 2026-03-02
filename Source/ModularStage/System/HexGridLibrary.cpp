#include "ModularStage/System/HexGridLibrary.h"

FVector UHexGridLibrary::HexToWorld(const FHexCoord& Coord, float Z)
{
    using namespace ModularStageDefine::HexGrid;

    // Pointy Top Hexagon 변환 공식
    // x = size * sqrt(3) * (q + r/2)
    // y = size * 3/2 * r
    float x = TILE_SIZE * (FMath::Sqrt(3.0f) * Coord.Q + (FMath::Sqrt(3.0f) / 2.0f) * Coord.R);
    float y = VERTICAL_SPACING * Coord.R;

    return FVector(x, y, Z);
}

FHexCoord UHexGridLibrary::WorldToHex(const FVector& WorldPos)
{
    using namespace ModularStageDefine::HexGrid;

    // x, y를 q, r로 역변환
    // q = (sqrt(3)/3 * x - 1/3 * y) / size
    // r = (2/3 * y) / size
    float q = (FMath::Sqrt(3.0f) / 3.0f * WorldPos.X - 1.0f / 3.0f * WorldPos.Y) / TILE_SIZE;
    float r = (2.0f / 3.0f * WorldPos.Y) / TILE_SIZE;

    return HexRound(q, r);
}

int32 UHexGridLibrary::GetDistance(const FHexCoord& A, const FHexCoord& B)
{
    // Cube Distance: (|dq| + |dr| + |ds|) / 2
    return (FMath::Abs(A.Q - B.Q) + FMath::Abs(A.R - B.R) + FMath::Abs(A.GetS() - B.GetS())) / 2;
}

TArray<FHexCoord> UHexGridLibrary::GetNeighbors(const FHexCoord& Origin)
{
    static const FHexCoord Directions[6] = {
        FHexCoord(1, 0), FHexCoord(1, -1), FHexCoord(0, -1),
        FHexCoord(-1, 0), FHexCoord(-1, 1), FHexCoord(0, 1)
    };

    TArray<FHexCoord> Neighbors;
    for (int32 i = 0; i < 6; ++i)
    {
        Neighbors.Add(Origin + Directions[i]);
    }
    return Neighbors;
}

FHexCoord UHexGridLibrary::HexRound(float q, float r)
{
    float s = -q - r;
    
    int32 iq = FMath::RoundToInt(q);
    int32 ir = FMath::RoundToInt(r);
    int32 is = FMath::RoundToInt(s);

    float dq = FMath::Abs(iq - q);
    float dr = FMath::Abs(ir - r);
    float ds = FMath::Abs(is - s);

    if (dq > dr && dq > ds)
    {
        iq = -ir - is;
    }
    else if (dr > ds)
    {
        ir = -iq - is;
    }
    // ds가 가장 크면 is는 -iq - ir이 되지만, Axial 좌표계에서는 Q, R만 중요함

    return FHexCoord(iq, ir);
}

int32 UHexGridLibrary::CoordToIndex(const FHexCoord& Coord, int32 GridWidth)
{
    // 단순한 2D 격자 맵핑 (Offset Coordinates 기반 맵에서 주로 사용)
    // 여기서는 간단히 구현하며, 필요 시 수정 예정
    return Coord.Q + Coord.R * GridWidth;
}

FHexCoord UHexGridLibrary::IndexToCoord(int32 Index, int32 GridWidth)
{
    if (GridWidth == 0) return FHexCoord(0, 0);
    return FHexCoord(Index % GridWidth, Index / GridWidth);
}
