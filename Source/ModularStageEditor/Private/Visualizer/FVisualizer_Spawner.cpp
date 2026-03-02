// Copyright Epic Games, Inc. All Rights Reserved.

#include "Visualizer/FVisualizer_Spawner.h"
#include "Mission/Component/MissionPropsComponent_Spawner.h"
#include "Mission/MissionPrefab.h"
#include "ModularStage/System/HexGridLibrary.h"
#include "SceneManagement.h"
#include "EditorViewportClient.h" // Required for FEditorViewportClient
#include "SPrefabEditorViewport.h" // [필수] 커스텀 뷰포트 형변환을 위해

IMPLEMENT_HIT_PROXY(HSpawnerVisProxy, HComponentVisProxy);

// Point type names
namespace SpawnerPointType
{
	static const FName Spawn = FName(TEXT("Spawn"));
	static const FName Patrol = FName(TEXT("Patrol"));
}

FVisualizer_Spawner::FVisualizer_Spawner()
{
	SelectedPointIndex = INDEX_NONE;
	SelectedPointType = NAME_None;
}

FVisualizer_Spawner::~FVisualizer_Spawner()
{
}

void FVisualizer_Spawner::DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI)
{
	const UMissionPropsComponent_Spawner* SpawnerComponent = Cast<const UMissionPropsComponent_Spawner>(Component);
	if (!SpawnerComponent || !SpawnerComponent->GetOwner())
	{
		return;
	}

	AMissionPrefab* ParentPrefab = Cast<AMissionPrefab>(SpawnerComponent->GetOwner());
	if (!ParentPrefab) return;

	const FTransform& ComponentTransform = SpawnerComponent->GetComponentTransform();
	
	// [단일화] 데이터 기반 자신의 타일 위치 (CenterLocation)
	const FVector MyTileLoc = ParentPrefab->GetTileLocation(SpawnerComponent->GetHexTileIndex());

	// Draw SpawnTileIndices
	for (int32 i = 0; i < SpawnerComponent->SpawnTileIndices.Num(); ++i)
	{
		int32 TileIdx = SpawnerComponent->SpawnTileIndices[i];
		// [단일화] 대상 타일 중심 - 내 타일 중심 = 정확한 상대 오프셋
		const FVector RelativeOffset = ParentPrefab->GetTileLocation(TileIdx) - MyTileLoc;
		const FVector WorldSpawnPoint = ComponentTransform.TransformPosition(RelativeOffset);

		PDI->SetHitProxy(new HSpawnerVisProxy(SpawnerComponent, i, SpawnerPointType::Spawn));
		DrawWireSphere(PDI, WorldSpawnPoint, FLinearColor::Red, 20.f, 12, SDPG_Foreground);
		PDI->SetHitProxy(nullptr);
	}

	// Draw PatrolTileIndices
	for (int32 i = 0; i < SpawnerComponent->PatrolTileIndices.Num(); ++i)
	{
		int32 TileIdx = SpawnerComponent->PatrolTileIndices[i];
		// [단일화] 대상 타일 중심 - 내 타일 중심 = 정확한 상대 오프셋
		const FVector RelativeOffset = ParentPrefab->GetTileLocation(TileIdx) - MyTileLoc;
		const FVector WorldPatrolPoint = ComponentTransform.TransformPosition(RelativeOffset);

		PDI->SetHitProxy(new HSpawnerVisProxy(SpawnerComponent, i, SpawnerPointType::Patrol));
		DrawWireSphere(PDI, WorldPatrolPoint, FLinearColor::Green, 20.f, 12, SDPG_Foreground);
		PDI->SetHitProxy(nullptr);

		if (i > 0)
		{
			int32 PrevTileIdx = SpawnerComponent->PatrolTileIndices[i - 1];
			const FVector PrevRelativeOffset = ParentPrefab->GetTileLocation(PrevTileIdx) - MyTileLoc;
			const FVector WorldPrevPoint = ComponentTransform.TransformPosition(PrevRelativeOffset);
			PDI->DrawLine(WorldPrevPoint, WorldPatrolPoint, FLinearColor::Green, SDPG_Foreground);
		}
	}
}

bool FVisualizer_Spawner::VisProxyHandleClick(FEditorViewportClient* InViewportClient, HComponentVisProxy* VisProxy, const FViewportClick& Click)
{
	SelectedPointIndex = INDEX_NONE;
	SelectedPointType = NAME_None;

	if (Click.GetKey() != EKeys::LeftMouseButton)
	{
		return FVisualizer_Base::VisProxyHandleClick(InViewportClient, VisProxy, Click);
	}

	if (false == VisProxy->Component.IsValid())
	{
		return FVisualizer_Base::VisProxyHandleClick(InViewportClient, VisProxy, Click);
	}

	if (HSpawnerVisProxy* SpawnerProxy = HitProxyCast<HSpawnerVisProxy>(VisProxy))
	{
		SelectedPointIndex = SpawnerProxy->PointIndex;
		SelectedPointType = SpawnerProxy->PointType;
		return true;
	}

	return FVisualizer_Base::VisProxyHandleClick(InViewportClient, VisProxy, Click);
}

bool FVisualizer_Spawner::GetWidgetLocation(const FEditorViewportClient* ViewportClient, FVector& OutLocation) const
{
	if (SelectedPointIndex != INDEX_NONE)
	{
		const UMissionPropsComponent_Spawner* SpawnerComponent = Cast<const UMissionPropsComponent_Spawner>(GetEditedComponent());
		if (SpawnerComponent && SpawnerComponent->GetOwner())
		{
			AMissionPrefab* ParentPrefab = Cast<AMissionPrefab>(SpawnerComponent->GetOwner());
			const FTransform& ComponentTransform = SpawnerComponent->GetComponentTransform();

			if (SelectedPointType == SpawnerPointType::Spawn && SpawnerComponent->SpawnTileIndices.IsValidIndex(SelectedPointIndex))
			{
				int32 TileIdx = SpawnerComponent->SpawnTileIndices[SelectedPointIndex];
				OutLocation = ComponentTransform.TransformPosition(ParentPrefab->GetTileLocation(TileIdx));
				return true;
			}
			if (SelectedPointType == SpawnerPointType::Patrol && SpawnerComponent->PatrolTileIndices.IsValidIndex(SelectedPointIndex))
			{
				int32 TileIdx = SpawnerComponent->PatrolTileIndices[SelectedPointIndex];
				OutLocation = ComponentTransform.TransformPosition(ParentPrefab->GetTileLocation(TileIdx));
				return true;
			}
		}
	}
	return false;
}

bool FVisualizer_Spawner::HandleInputDelta(FEditorViewportClient* ViewportClient, FViewport* Viewport, FVector& DeltaTranslate, FRotator& DeltaRotate, FVector& DeltaScale)
{
	if (SelectedPointIndex != INDEX_NONE)
	{
		UMissionPropsComponent_Spawner* SpawnerComponent = Cast<UMissionPropsComponent_Spawner>(const_cast<UActorComponent*>(GetEditedComponent()));
		if (SpawnerComponent && SpawnerComponent->GetOwner())
		{
			AMissionPrefab* ParentPrefab = Cast<AMissionPrefab>(SpawnerComponent->GetOwner());
			const FTransform& ComponentTransform = SpawnerComponent->GetComponentTransform();

			FVector CurrentWidgetLoc;
			GetWidgetLocation(ViewportClient, CurrentWidgetLoc);

			FVector NewWorldLoc = CurrentWidgetLoc + DeltaTranslate;
			FVector LocalLoc = ComponentTransform.InverseTransformPosition(NewWorldLoc);

			// 7개 타일 중 가장 가까운 인덱스 찾기
			int32 NewTileIdx = 0;
			float MinDist = FLT_MAX;
			for (int32 i = 0; i < 7; ++i)
			{
				float Dist = FVector::DistSquared(LocalLoc, ParentPrefab->GetTileLocation(i));
				if (Dist < MinDist)
				{
					MinDist = Dist;
					NewTileIdx = i;
				}
			}

			if (SelectedPointType == SpawnerPointType::Spawn && SpawnerComponent->SpawnTileIndices.IsValidIndex(SelectedPointIndex))
			{
				if (SpawnerComponent->SpawnTileIndices[SelectedPointIndex] != NewTileIdx)
				{
					SpawnerComponent->SpawnTileIndices[SelectedPointIndex] = NewTileIdx;
					SpawnerComponent->MarkRenderStateDirty();
					return true;
				}
			}
			else if (SelectedPointType == SpawnerPointType::Patrol && SpawnerComponent->PatrolTileIndices.IsValidIndex(SelectedPointIndex))
			{
				if (SpawnerComponent->PatrolTileIndices[SelectedPointIndex] != NewTileIdx)
				{
					SpawnerComponent->PatrolTileIndices[SelectedPointIndex] = NewTileIdx;
					SpawnerComponent->MarkRenderStateDirty();
					return true;
				}
			}
		}
	}
	return false;
}
