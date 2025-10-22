#pragma once

#include "CoreMinimal.h"
#include "PrimitiveSceneProxy.h"

class UMissionPropsComponent_Spawner;
struct FMeshBatchAndRelevance;

class FSpawnerSceneProxy : public FPrimitiveSceneProxy
{
public:
    SIZE_T GetTypeHash() const override;
    FSpawnerSceneProxy(const UMissionPropsComponent_Spawner* InComponent);
    virtual void GetDynamicMeshElements(const TArray<const FSceneView*>& Views, const FSceneViewFamily& ViewFamily, uint32 VisibilityMap, FMeshElementCollector& Collector) const override;
    virtual FPrimitiveViewRelevance GetViewRelevance(const FSceneView* View) const override;
    virtual uint32 GetMemoryFootprint() const override { return (sizeof(*this) + GetAllocatedSize()); }
    SIZE_T GetAllocatedSize() const { return FPrimitiveSceneProxy::GetAllocatedSize(); }

private:
    TArray<FVector> SpawnPoints;
    TArray<FVector> PatrolPoints;
    bool IsShowDebug = true;
};