#pragma once

#include "CoreMinimal.h"
#include "HitProxies.h"
#include "Visualizer/FVisualizer_Base.h"

struct HSpawnerVisProxy : public HComponentVisProxy
{
	DECLARE_HIT_PROXY();

	int32 PointIndex;
	FName PointType; // "Spawn" or "Patrol"

	HSpawnerVisProxy(const UActorComponent* InComponent, int32 InPointIndex, FName InPointType)
		:HComponentVisProxy(InComponent, HPP_Wireframe)
	{
		PointIndex = InPointIndex;
		PointType = InPointType;
	}
};


class FVisualizer_Spawner : public FVisualizer_Base
{
public:
	FVisualizer_Spawner();
	virtual ~FVisualizer_Spawner();

	//~ Begin FComponentVisualizer Interface
	virtual void DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI) override;
	virtual bool VisProxyHandleClick(FEditorViewportClient* InViewportClient, HComponentVisProxy* VisProxy, const FViewportClick& Click) override;
	virtual bool GetWidgetLocation(const FEditorViewportClient* ViewportClient, FVector& OutLocation) const override;
	virtual bool HandleInputDelta(FEditorViewportClient* ViewportClient, FViewport* Viewport, FVector& DeltaTranslate, FRotator& DeltaRotate, FVector& DeltaScale) override;
	//~ End FComponentVisualizer Interface

protected:
	// Track selected point
	int32 SelectedPointIndex;
	FName SelectedPointType;

};
