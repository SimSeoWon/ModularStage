#include "FSpawnerSceneProxy.h"
#include "Engine/Engine.h" // GEngine ���
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

    // ���Ͻ��� Local-to-World Ʈ������ ��Ʈ������ �����ɴϴ�.
    const FMatrix& localToWorld = GetLocalToWorld();

    FTexture* texture = nullptr;
    if (SpriteIcon && SpriteIcon->GetResource())
    {
        texture = SpriteIcon->GetResource();
    }
    else if (GEngine->DefaultTexture && GEngine->DefaultTexture->GetResource())
    {
        // ������ �������� ������ ���� �⺻ ������ (S_Actor)�� ����մϴ�.
        texture = GEngine->DefaultTexture->GetResource();
    }

    for (int32 i = 0; i < Views.Num(); i++)
    {
        if (VisibilityMap & (1 << i))
        {
            const FSceneView* view = Views[i];
            FPrimitiveDrawInterface* pid = Collector.GetPDI(i);

            // 2. �ؽ�ó ���ҽ��� ��ȿ�ϸ� �׸��ϴ�.
            if (texture)
            {
                // ������Ʈ�� ���� ��ġ�� �����ɴϴ�.
                FVector location = localToWorld.GetOrigin();
                location.Z += 32.0f;

                // �⺻ ũ�⸦ ���մϴ�. (��: 32x32)
                const float size = 32.0f;

                // ���� ���¿� ���� ������ ���մϴ�. (����̾�� �ؽ�ó ������ ���� ����)
                const FLinearColor color = GetViewSelectionColor(
                    FLinearColor::White, *view, IsSelected(), IsHovered(), false, IsIndividuallySelected()
                );

                // ��������Ʈ�� �׸��ϴ�.
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
    // ���õǾ��� ���� �׸��ų�, �׻� ���̵��� ����
    const bool bProxyVisible = IsSelected() || IsShowDebug;

    // ����Ʈ�� 'Show' �÷��׿��� 'Sprites' �Ǵ� 'Collision'�� ���� ���� �� ���̵��� �� �� �ֽ��ϴ�.
    // ���⼭�� 'Collision' �÷��׸� �������� ���ø� ����ϴ�.
    const bool bShowForCollision = View->Family->EngineShowFlags.Collision && IsCollisionEnabled();

    FPrimitiveViewRelevance Result;
    // (���̵��� �����Ǿ���(IsShown) ���õǾ��ų�(bProxyVisible)) �Ǵ� (�ݸ��� ǥ�ð� �����ִٸ�) �׸��ϴ�.
    Result.bDrawRelevance = (IsShown(View) && bProxyVisible) || bShowForCollision;
    Result.bDynamicRelevance = true; // GetDynamicMeshElements�� ����ϹǷ� true
    Result.bShadowRelevance = IsShadowCast(View);
    Result.bEditorPrimitiveRelevance = UseEditorCompositing(View);
    return Result;
}



