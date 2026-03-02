#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ModularStage/ModularStageDefine.h"
#include "HexGridLibrary.generated.h"

/**
 * 육각형 좌표를 표현하는 구조체 (Axial Coordinate: Q, R)
 * S(Cube coordinate)는 -Q -R 로 계산 가능
 */
USTRUCT(BlueprintType)
struct MODULARSTAGE_API FHexCoord
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hex")
    int32 Q = 0;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hex")
    int32 R = 0;

    FHexCoord() : Q(0), R(0) {}
    FHexCoord(int32 InQ, int32 InR) : Q(InQ), R(InR) {}

    bool operator==(const FHexCoord& Other) const { return Q == Other.Q && R == Other.R; }
    bool operator!=(const FHexCoord& Other) const { return !(*this == Other); }

    FHexCoord operator+(const FHexCoord& Other) const { return FHexCoord(Q + Other.Q, R + Other.R); }
    FHexCoord operator-(const FHexCoord& Other) const { return FHexCoord(Q - Other.Q, R - Other.R); }

    /** S(Cube) 값을 계산하여 반환 */
    int32 GetS() const { return -Q - R; }

    FString ToString() const { return FString::Printf(TEXT("(Q:%d, R:%d)"), Q, R); }
};

/**
 * 육각형 그리드 계산을 위한 수학 유틸리티 라이브러리
 */
UCLASS()
class MODULARSTAGE_API UHexGridLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    /** Axial(Q, R) 좌표를 월드(X, Y) 좌표로 변환 (Pointy Top 기준) */
    UFUNCTION(BlueprintPure, Category = "HexGrid")
    static FVector HexToWorld(const FHexCoord& Coord, float Z = 0.0f);

    /** 월드(X, Y) 좌표를 가장 가까운 Axial(Q, R) 좌표로 변환 */
    UFUNCTION(BlueprintPure, Category = "HexGrid")
    static FHexCoord WorldToHex(const FVector& WorldPos);

    /** 두 육각형 타일 간의 거리(Step count) 계산 */
    UFUNCTION(BlueprintPure, Category = "HexGrid")
    static int32 GetDistance(const FHexCoord& A, const FHexCoord& B);

    /** 특정 타일의 인접 타일(Neighbor) 좌표 목록 반환 */
    UFUNCTION(BlueprintPure, Category = "HexGrid")
    static TArray<FHexCoord> GetNeighbors(const FHexCoord& Origin);

    /** 실수형 좌표를 정수형 Axial 좌표로 반올림 (Hex Rounding) */
    static FHexCoord HexRound(float Q, float R);

    /** 타일 인덱스(Linear Index)와 Axial 좌표 간의 변환 (간이 구현, 맵 크기 의존적일 수 있음) */
    static int32 CoordToIndex(const FHexCoord& Coord, int32 GridWidth);
    static FHexCoord IndexToCoord(int32 Index, int32 GridWidth);
};
