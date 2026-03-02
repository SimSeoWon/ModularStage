#pragma once

#include "CoreMinimal.h"

#include "PrimitiveSceneProxy.h"

class FTexture;
class FMeshElementCollector;
class UHexGridVisualComponent;
struct FMeshBatchAndRelevance;

/**
 * 에디터에서 육각형 타일들을 렌더링하는 실제 프록시 클래스
 */
class FHexGridSceneProxy : public FPrimitiveSceneProxy
{
public:
	FHexGridSceneProxy(UHexGridVisualComponent* InComponent);

	virtual void GetDynamicMeshElements(const TArray<const FSceneView*>& Views, const FSceneViewFamily& ViewFamily, uint32 VisibilityMap, FMeshElementCollector& Collector) const override;
	
	virtual FPrimitiveViewRelevance GetViewRelevance(const FSceneView* View) const override;

	virtual SIZE_T GetTypeHash() const override;

	virtual uint32 GetMemoryFootprint() const override { return sizeof(*this) + GetAllocatedSize(); }
	uint32 GetAllocatedSize() const { return FPrimitiveSceneProxy::GetAllocatedSize(); }

private:
	TArray<FHexTileConfig> TileConfigs;
	bool bShowInGame = false;
};