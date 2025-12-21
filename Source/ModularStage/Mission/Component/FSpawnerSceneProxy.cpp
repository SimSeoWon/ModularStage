#include "FSpawnerSceneProxy.h"
#include "Engine/Engine.h"
#include "SceneManagement.h"
#include "TextureResource.h"
#include "MissionPropsComponent_Spawner.h"

SIZE_T FSpawnerSceneProxy::GetTypeHash() const
{
    static size_t UniquePointer;
    return reinterpret_cast<size_t>(&UniquePointer);
}

FSpawnerSceneProxy::FSpawnerSceneProxy(const UMissionPropsComponent_Spawner* InComponent)
    : FPrimitiveSceneProxy(InComponent)
{
    //UE_LOG(LogTemp, Warning, TEXT("FSpawnerSceneProxy constructor called!"));
    SpawnPoints.Append(InComponent->SpawnPoints);
    PatrolPoints.Append(InComponent->PatrolPoints);
    IsShowDebug = InComponent->IsShowDebug;
    
    // [수정] UObject인 SpriteIcon을 직접 저장하지 않고, 리소스만 추출해서 캐싱
    TextureResource = nullptr;
    if (InComponent->SpriteIcon && InComponent->SpriteIcon->GetResource())
    {
        TextureResource = InComponent->SpriteIcon->GetResource();
    }
    else if (GEngine->DefaultTexture)
    {
        TextureResource = GEngine->DefaultTexture->GetResource();
    }
}

void FSpawnerSceneProxy::GetDynamicMeshElements(const TArray<const FSceneView*>& Views, const FSceneViewFamily& ViewFamily, uint32 VisibilityMap, FMeshElementCollector& Collector) const
{
    QUICK_SCOPE_CYCLE_COUNTER(STAT_SpawnerSceneProxy_GetDynamicMeshElements);

    //Local-to-World
    const FMatrix& localToWorld = GetLocalToWorld();

    // [수정] 미리 캐싱된 TextureResource 사용
    // UObject(SpriteIcon)에 접근하지 않으므로 GC 크래시 해결
    FTexture* texture = TextureResource;

    for (int32 i = 0; i < Views.Num(); i++)
    {
        if (VisibilityMap & (1 << i))
        {
            const FSceneView* view = Views[i];
            FPrimitiveDrawInterface* pid = Collector.GetPDI(i);

            if (texture)
            {
                FVector location = localToWorld.GetOrigin();
                location.Z += 32.0f;
                const float size = 32.0f;
                const FLinearColor color = GetViewSelectionColor(
                    FLinearColor::White, *view, IsSelected(), IsHovered(), false, IsIndividuallySelected()
                );

                pid->DrawSprite(location, size, size, texture,
                    color, SDPG_World, 0.0f, 0.0f,  0.0f, 0.0f);
            }

            // Draw SpawnPoints as red points
            for (const FVector& Point : SpawnPoints)
            {
                FVector point = localToWorld.TransformPosition(Point);
                pid->DrawPoint(point, FLinearColor::Red, 10.0f, SDPG_World);
            }

            // Draw PatrolPoints as green lines
            for (int32 j = 0; j < PatrolPoints.Num() - 1; ++j)
            {
                FVector start = localToWorld.TransformPosition(PatrolPoints[j]);
                FVector end = localToWorld.TransformPosition(PatrolPoints[j + 1]);
                pid->DrawLine(start, end, FLinearColor::Green, SDPG_World, 2.0f);
            }
        }
    }
}

FPrimitiveViewRelevance FSpawnerSceneProxy::GetViewRelevance(const FSceneView* View) const
{
    const bool bProxyVisible = IsSelected() || IsShowDebug;
    const bool bShowForCollision = View->Family->EngineShowFlags.Collision && IsCollisionEnabled();

    FPrimitiveViewRelevance Result;
    Result.bDrawRelevance = (IsShown(View) && bProxyVisible) || bShowForCollision;
    Result.bDynamicRelevance = true; // GetDynamicMeshElements
    Result.bShadowRelevance = IsShadowCast(View);
    Result.bEditorPrimitiveRelevance = UseEditorCompositing(View);
    return Result;
}



