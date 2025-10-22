#include "FSpawnerSceneProxy.h"
#include "MissionPropsComponent_Spawner.h"
#include "SceneManagement.h"

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
}

void FSpawnerSceneProxy::GetDynamicMeshElements(const TArray<const FSceneView*>& Views, const FSceneViewFamily& ViewFamily, uint32 VisibilityMap, FMeshElementCollector& Collector) const
{
    QUICK_SCOPE_CYCLE_COUNTER(STAT_SpawnerSceneProxy_GetDynamicMeshElements);

    // ���Ͻ��� Local-to-World Ʈ������ ��Ʈ������ �����ɴϴ�.
    const FMatrix& localToWorld = GetLocalToWorld();

    for (int32 ViewIndex = 0; ViewIndex < Views.Num(); ViewIndex++)
    {
        if (VisibilityMap & (1 << ViewIndex))
        {
            const FSceneView* View = Views[ViewIndex];
            FPrimitiveDrawInterface* PDI = Collector.GetPDI(ViewIndex);

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



