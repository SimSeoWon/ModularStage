#include "HexGridVisualComponent.h"
#include "PrimitiveSceneProxy.h"

#include "ModularStage/Mission/MissionPrefab.h"
#include "ModularStage/System/HexGridLibrary.h"
#include "ModularStage/ModularStageDefine.h"
#include "FHexGridSceneProxy.h"

UHexGridVisualComponent::UHexGridVisualComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetCastShadow(false);
	bHiddenInGame = true; // 게임 실행 중에는 숨김
}

void UHexGridVisualComponent::UpdateTileConfigs(const TArray<FHexTileConfig>& InConfigs)
{
	TileConfigs = InConfigs;
	MarkRenderStateDirty(); // 다시 그리기 요청
}

FPrimitiveSceneProxy* UHexGridVisualComponent::CreateSceneProxy()
{
	return new FHexGridSceneProxy(this);
}

FBoxSphereBounds UHexGridVisualComponent::CalcBounds(const FTransform& LocalToWorld) const
{
	// 대략적인 7개 타일 영역의 크기 계산 (SEPTET 크기)
	float Radius = ModularStageDefine::HexGrid::TILE_SIZE * 3.0f; 
	return FBoxSphereBounds(LocalToWorld.GetLocation(), FVector(Radius), Radius);
}
