#pragma once

#include "CoreMinimal.h"
#include "SEditorViewport.h"
#include "EditorViewportClient.h"
#include "AdvancedPreviewScene.h"

class AMissionPrefab;
DECLARE_DELEGATE_OneParam(FOnSelectionChanged, UObject*);

/**
 * Custom Viewport Client for the Prefab Editor.
 * Handles selection and gizmo interaction within the preview scene.
 */
class FPrefabEditorViewportClient : public FEditorViewportClient
{
public:
	FPrefabEditorViewportClient(FAdvancedPreviewScene& InPreviewScene, const TWeakPtr<SEditorViewport>& InEditorViewportWidget);
	virtual void Tick(float DeltaSeconds) override;

	// Interaction Overrides
	virtual void ProcessClick(FSceneView& View, HHitProxy* HitProxy, FKey Key, EInputEvent Event, uint32 HitX, uint32 HitY) override;
	virtual bool InputWidgetDelta(FViewport* InViewport, EAxisList::Type CurrentAxis, FVector& Drag, FRotator& Rot, FVector& Scale) override;
	virtual void Draw(const FSceneView* View, FPrimitiveDrawInterface* PDI) override;
	virtual FVector GetWidgetLocation() const override;
	virtual FMatrix GetWidgetCoordSystem() const override;
	virtual UE::Widget::EWidgetMode GetWidgetMode() const override;

	FOnSelectionChanged OnSelectionChanged;

	void SelectObject(UObject* InObject, bool bInReplaceSelection);
	void ClearSelection();
	void SetPrefabActor(AMissionPrefab* InPrefabActor);

	UActorComponent* GetVisualizedComponent() const { return VisualizedComponent.Get(); }

private:
	/** The root prefab actor being edited. Used to drive visualizer drawing. */
	TWeakObjectPtr<class AMissionPrefab> PrefabActor;
	
	/** The set of objects currently selected in the preview scene */
	TSet<TWeakObjectPtr<UObject>> SelectedObjects;

	// [추가] 현재 활성화된 컴포넌트 비주얼라이저 추적용
	TWeakObjectPtr<UActorComponent> VisualizedComponent;
};

/**
 * A specialized viewport widget for the Prefab Editor.
 */
class SPrefabEditorViewport : public SEditorViewport
{
public:
	SLATE_BEGIN_ARGS(SPrefabEditorViewport) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, TSharedPtr<FAdvancedPreviewScene> InPreviewScene);
	virtual ~SPrefabEditorViewport();

	/** Explicitly cleans up the viewport client to prevent crashes on destruction */
	void Shutdown();

	FOnSelectionChanged& GetOnSelectionChanged() 
	{ 
		// Return the client's delegate directly. 
		// Note: ViewportClient must be valid when this is called.
		check(ViewportClient.IsValid());
		return ViewportClient->OnSelectionChanged; 
	}

	/** Selects an object in the preview scene */
	void SelectObject(UObject* InObject, bool bInReplaceSelection)
	{
		if (ViewportClient.IsValid())
		{
			ViewportClient->SelectObject(InObject, bInReplaceSelection);
		}
	}

	/** Clears current selection in the preview scene */
	void ClearSelection()
	{
		if (ViewportClient.IsValid())
		{
			ViewportClient->ClearSelection();
		}
	}

	TSharedPtr<FPrefabEditorViewportClient> GetPrefabEditorViewportClient() const { return ViewportClient; }

protected:
	virtual TSharedRef<FEditorViewportClient> MakeEditorViewportClient() override;

private:
	TSharedPtr<FAdvancedPreviewScene> PreviewScene;
	TSharedPtr<FPrefabEditorViewportClient> ViewportClient;
};
