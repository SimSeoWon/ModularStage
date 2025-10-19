// Copyright Epic Games, Inc. All Rights Reserved.

#include "Visualizer/FVisualizer_Spawner.h"
#include "Mission/Component/MissionPropsComponent_Spawner.h"
#include "SceneManagement.h"
#include "EditorViewportClient.h" // Required for FEditorViewportClient

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
	if (!SpawnerComponent)
	{
		return;
	}

	const FTransform& ComponentTransform = SpawnerComponent->GetComponentTransform();
	const FVector ComponentLocation = ComponentTransform.GetLocation();

	// Draw SpawnPoints
	for (int32 i = 0; i < SpawnerComponent->SpawnPoints.Num(); ++i)
	{
		const FVector& SpawnPoint = SpawnerComponent->SpawnPoints[i];
		const FVector WorldSpawnPoint = ComponentTransform.TransformPosition(SpawnPoint);

		PDI->SetHitProxy(new HSpawnerVisProxy(SpawnerComponent, i, SpawnerPointType::Spawn));
		DrawWireSphere(PDI, WorldSpawnPoint, FLinearColor::Blue, 20.f, 12, SDPG_World);
		PDI->SetHitProxy(nullptr);
	}

	// Draw PatrolPoints
	for (int32 i = 0; i < SpawnerComponent->PatrolPoints.Num(); ++i)
	{
		const FVector& PatrolPoint = SpawnerComponent->PatrolPoints[i];
		const FVector WorldPatrolPoint = ComponentTransform.TransformPosition(PatrolPoint);

		PDI->SetHitProxy(new HSpawnerVisProxy(SpawnerComponent, i, SpawnerPointType::Patrol));
		DrawWireSphere(PDI, WorldPatrolPoint, FLinearColor::Red, 20.f, 12, SDPG_World);
		PDI->SetHitProxy(nullptr);

		if (i > 0)
		{
			const FVector& PrevPoint = SpawnerComponent->PatrolPoints[i - 1];
			const FVector WorldPrevPoint = ComponentTransform.TransformPosition(PrevPoint);
			PDI->DrawLine(WorldPrevPoint, WorldPatrolPoint, FLinearColor::Red, SDPG_World);
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
		if (const UMissionPropsComponent_Spawner* SpawnerComponent = Cast<const UMissionPropsComponent_Spawner>(GetEditedComponent()))
		{
			const FTransform& ComponentTransform = SpawnerComponent->GetComponentTransform();
			if (SelectedPointType == SpawnerPointType::Spawn && SpawnerComponent->SpawnPoints.IsValidIndex(SelectedPointIndex))
			{
				OutLocation = ComponentTransform.TransformPosition(SpawnerComponent->SpawnPoints[SelectedPointIndex]);
				return true;
			}
			if (SelectedPointType == SpawnerPointType::Patrol && SpawnerComponent->PatrolPoints.IsValidIndex(SelectedPointIndex))
			{
				OutLocation = ComponentTransform.TransformPosition(SpawnerComponent->PatrolPoints[SelectedPointIndex]);
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
		if (UMissionPropsComponent_Spawner* SpawnerComponent = Cast<UMissionPropsComponent_Spawner>(GetEditedComponent()))
		{
			const FTransform& ComponentTransform = SpawnerComponent->GetComponentTransform();

			// Convert world-space delta to local-space
			const FVector LocalDelta = ComponentTransform.InverseTransformVector(DeltaTranslate);

			if (SelectedPointType == SpawnerPointType::Spawn && SpawnerComponent->SpawnPoints.IsValidIndex(SelectedPointIndex))
			{
				SpawnerComponent->SpawnPoints[SelectedPointIndex] += LocalDelta;
				SpawnerComponent->MarkPackageDirty();
				return true;
			}
			if (SelectedPointType == SpawnerPointType::Patrol && SpawnerComponent->PatrolPoints.IsValidIndex(SelectedPointIndex))
			{
				SpawnerComponent->PatrolPoints[SelectedPointIndex] += LocalDelta;
				SpawnerComponent->MarkPackageDirty();
				return true;
			}
		}
	}
	return false;
}
