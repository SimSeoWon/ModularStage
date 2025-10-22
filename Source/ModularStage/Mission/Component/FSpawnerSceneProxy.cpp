#include "FSpawnerSceneProxy.h"
#include "Engine/Engine.h" // GEngine 사용
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
    UE_LOG(LogTemp, Warning, TEXT("FSpawnerSceneProxy constructor called!"));
    SpawnPoints.Append(InComponent->SpawnPoints);
    PatrolPoints.Append(InComponent->PatrolPoints);
    IsShowDebug = InComponent->IsShowDebug;
    SpriteIcon = InComponent->SpriteIcon;
}

void FSpawnerSceneProxy::GetDynamicMeshElements(const TArray<const FSceneView*>& Views, const FSceneViewFamily& ViewFamily, uint32 VisibilityMap, FMeshElementCollector& Collector) const
{
    QUICK_SCOPE_CYCLE_COUNTER(STAT_SpawnerSceneProxy_GetDynamicMeshElements);

    // 프록시의 Local-to-World 트랜스폼 매트릭스를 가져옵니다.
    const FMatrix& localToWorld = GetLocalToWorld();

    FTexture* TextureResource = nullptr;
    if (SpriteIcon && SpriteIcon->GetResource())
    {
        TextureResource = SpriteIcon->GetResource();
    }
    else if (GEngine->DefaultTexture && GEngine->DefaultTexture->GetResource())
    {
        // 설정된 아이콘이 없으면 엔진 기본 아이콘 (S_Actor)을 사용합니다.
        TextureResource = GEngine->DefaultTexture->GetResource();
    }

    for (int32 ViewIndex = 0; ViewIndex < Views.Num(); ViewIndex++)
    {
        if (VisibilityMap & (1 << ViewIndex))
        {
            const FSceneView* View = Views[ViewIndex];
            FPrimitiveDrawInterface* PDI = Collector.GetPDI(ViewIndex);

            // 2. 텍스처 리소스가 유효하면 그립니다.
            if (TextureResource)
            {
                // 컴포넌트의 월드 위치를 가져옵니다.
                const FVector SpriteLocation = localToWorld.GetOrigin();

                // 기본 크기를 정합니다. (예: 32x32)
                const float SpriteSize = 32.0f;

                // 선택 상태에 따라 색상을 정합니다. (흰색이어야 텍스처 본연의 색이 나옴)
                const FLinearColor SpriteColor = GetViewSelectionColor(
                    FLinearColor::White, *View, IsSelected(), IsHovered(), false, IsIndividuallySelected()
                );

                // 스프라이트를 그립니다.
                PDI->DrawSprite( SpriteLocation, SpriteSize, SpriteSize, TextureResource,
                    SpriteColor, SDPG_World, 0.0f, 0.0f,  0.0f, 0.0f);
            }

            // Draw SpawnPoints as red points
            for (const FVector& Point : SpawnPoints)
            {
                FVector WorldPoint = localToWorld.TransformPosition(Point);
                PDI->DrawPoint(WorldPoint, FLinearColor::Red, 10.0f, SDPG_World);
            }

            // Draw PatrolPoints as green lines
            for (int32 i = 0; i < PatrolPoints.Num() - 1; ++i)
            {
                FVector WorldStart = localToWorld.TransformPosition(PatrolPoints[i]);
                FVector WorldEnd = localToWorld.TransformPosition(PatrolPoints[i + 1]);
                PDI->DrawLine(WorldStart, WorldEnd, FLinearColor::Green, SDPG_World, 2.0f);
            }
        }
    }
}

FPrimitiveViewRelevance FSpawnerSceneProxy::GetViewRelevance(const FSceneView* View) const
{
    // 선택되었을 때만 그리거나, 항상 보이도록 설정
    const bool bProxyVisible = IsSelected() || IsShowDebug;

    // 뷰포트의 'Show' 플래그에서 'Sprites' 또는 'Collision'이 켜져 있을 때 보이도록 할 수 있습니다.
    // 여기서는 'Collision' 플래그를 따르도록 예시를 만듭니다.
    const bool bShowForCollision = View->Family->EngineShowFlags.Collision && IsCollisionEnabled();

    FPrimitiveViewRelevance Result;
    // (보이도록 설정되었고(IsShown) 선택되었거나(bProxyVisible)) 또는 (콜리전 표시가 켜져있다면) 그립니다.
    Result.bDrawRelevance = (IsShown(View) && bProxyVisible) || bShowForCollision;
    Result.bDynamicRelevance = true; // GetDynamicMeshElements를 사용하므로 true
    Result.bShadowRelevance = IsShadowCast(View);
    Result.bEditorPrimitiveRelevance = UseEditorCompositing(View);
    return Result;
}



