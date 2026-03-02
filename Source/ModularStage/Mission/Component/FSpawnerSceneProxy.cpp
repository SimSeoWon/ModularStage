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

SIZE_T FSpawnerSceneProxy::GetAllocatedSize() const
{
	if (SpawnPoints.IsEmpty() && PatrolPoints.IsEmpty())
	{
		FPrimitiveSceneProxy::GetAllocatedSize();
	}

	return SpawnPoints.GetAllocatedSize() + PatrolPoints.GetAllocatedSize();
}

FSpawnerSceneProxy::FSpawnerSceneProxy(const UMissionPropsComponent_Spawner* InComponent)
	: FPrimitiveSceneProxy(InComponent)
{
	if (!InComponent) return;

	IsShowDebug = InComponent->IsShowDebug;

	// 부모 프리팹으로부터 타일 좌표 정보 추출
	AMissionPrefab* ParentPrefab = Cast<AMissionPrefab>(InComponent->GetOwner());
	if (ParentPrefab)
	{
		// 스포너 자신이 위치한 타일의 실제 데이터 좌표 (Z포함 CenterLocation)
		FVector MyTileLoc = ParentPrefab->GetTileLocation(InComponent->GetHexTileIndex());

		for (int32 Index : InComponent->SpawnTileIndices)
		{
			// [단일화] 대상 타일 중심 - 내 타일 중심 = 정확한 상대 오프셋
			SpawnPoints.Add(ParentPrefab->GetTileLocation(Index) - MyTileLoc);
		}

		for (int32 Index : InComponent->PatrolTileIndices)
		{
			// 순찰 지점도 동일하게 데이터 기반 상대 오프셋으로 저장
			PatrolPoints.Add(ParentPrefab->GetTileLocation(Index) - MyTileLoc);
		}
	}

	// [수정] UObject인 SpriteIcon을 직접 저장하지 않고, 리소스만 추출해서 캐싱
	TextureResource = nullptr;
	if (InComponent->SpriteIcon && InComponent->SpriteIcon->GetResource())
	{
		TextureResource = InComponent->SpriteIcon->GetResource();
	}
	else if (GEngine && GEngine->DefaultTexture)
	{
		TextureResource = GEngine->DefaultTexture->GetResource();
	}
}

void FSpawnerSceneProxy::GetDynamicMeshElements(const TArray<const FSceneView*>& Views, const FSceneViewFamily& ViewFamily, uint32 VisibilityMap, FMeshElementCollector& Collector) const
{
	///QUICK_SCOPE_CYCLE_COUNTER(STAT_SpawnerSceneProxy_GetDynamicMeshElements);

	if (!this) return;

	//Local-to-World
	const FMatrix& localToWorld = GetLocalToWorld();
	for (int32 i = 0; i < Views.Num(); i++)
	{
		if (VisibilityMap & (1 << i))
		{
			const FSceneView* view = Views[i];
			if (nullptr == view)
				continue;

			FPrimitiveDrawInterface* pid = Collector.GetPDI(i);
			if (nullptr == pid)
				continue;

			FTexture* texture = TextureResource;
			if (nullptr == texture || !texture->IsInitialized() || nullptr == texture->GetTextureRHI())
			{
				UE_LOG(LogTemp, Log, TEXT("invalid resource! : texture!"))
			}
			else 
			{
				//FVector location = localToWorld.GetOrigin();
				//location.Z += 32.0f;
				//const float size = 32.0f;
				//const FLinearColor color = GetViewSelectionColor(
				//	FLinearColor::White, *view, IsSelected(), IsHovered(), false, IsIndividuallySelected()
				//);

				//pid->DrawSprite(location, size, size, texture, color, SDPG_World, 0.0f, 0.0f, 0.0f, 0.0f);
			}

			if (false == SpawnPoints.IsEmpty())
			{
				// Draw SpawnPoints as red points
				for (const FVector& Point : SpawnPoints)
				{
					FVector point = localToWorld.TransformPosition(Point);
					pid->DrawPoint(point, FLinearColor::Red, 10.0f, SDPG_World);
				}
			}

			if (false == PatrolPoints.IsEmpty())
			{
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



