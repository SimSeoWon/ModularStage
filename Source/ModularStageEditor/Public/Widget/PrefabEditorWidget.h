#pragma once

#include "CoreMinimal.h"
#include "Widget/EditorWidgetBase.h"
#include "AdvancedPreviewScene.h"
#include "Widgets/Views/STreeView.h"
#include "PrefabEditorWidget.generated.h"

class FReply;
class UActorComponent;
class AMissionPrefab;
class SPrefabEditorViewport;
class UMissionPropsComponent_Spawner;

/** Data structure for items in the hierarchy tree view */
struct FPrefabEditorTreeItem
{
	/** The object this item represents (Actor or Component). Can be null for folders. */
	TWeakObjectPtr<UObject> Object;

	/** Display name for this item */
	FString DisplayName;

	/** Child items (Components of an Actor, etc.) */
	TArray<TSharedPtr<FPrefabEditorTreeItem>> Children;

	FPrefabEditorTreeItem(UObject* InObject) : Object(InObject)
	{
		if (InObject) DisplayName = InObject->GetName();
	}

	FPrefabEditorTreeItem(FString InName) : Object(nullptr), DisplayName(InName) {}
};

/**
 * Editor tool for creating and modifying Mission Prefabs.
 * Uses a Seed & Proxy Actor architecture for editing.
 */
UCLASS()
class MODULARSTAGEEDITOR_API UPrefabEditorWidget : public UEditorWidgetBase
{
	GENERATED_BODY()

public:
	static FName GetMenuName() { return FName(TEXT("Prefab Editor")); }
	static FText GetMenuDisplayName() { return FText::FromString(TEXT("Prefab Editor")); }
	static FText GetMenuTooltip() { return FText::FromString(TEXT("Tool for creating and editing AMissionPrefab assets")); }

	// Ensure this BP is created in the editor later
	static FString GetWidgetBlueprintPath() { return TEXT("/Game/nevercook/editor/menu/BP_PrefabEditorWidget.BP_PrefabEditorWidget"); }

protected:
	virtual void BeginDestroy() override;
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void OnEditorExit_Implement() override;

	/** Handles the click event for the 'Load' button. */
	void OnLoadClicked();

	/** Loads a prefab asset into the preview scene. */
	void LoadPrefab(const FAssetData& AssetData);

	/** Clones selected actors from the level into the preview scene. */
	void ImportSelectedFromLevel();

	/** Saves the current preview scene state back to the Seed's Blueprint asset. */
	void SavePrefab();

	/** Adds a Monster Spawner logic proxy actor to the preview scene. */
	void AddSpawner();

	/** Adds a Trigger logic proxy actor to the preview scene. */
	void AddTrigger();

	/** Spawns a preview of the selected spawner component's asset. */
	void OnPreviewButtonClicked();

	/** Clears all spawned preview actors from the scene. */
	void OnClearPreviewButtonClicked();

	/** Opens the mission task editor for the loaded prefab. */
	void OnEditMissionTasksClicked();

	void RefreshHierarchy();

	/** Gets the root prefab actor currently loaded in the editor. */
	AMissionPrefab* GetPrefabActor() const;

private:
	/** Helper to get the currently selected spawner component from the hierarchy view. */
	UMissionPropsComponent_Spawner* GetSelectedSpawnerComponent() const;

	/** Helper function to recursively build the hierarchy for scene components. */
	void AddChildrenToTree(USceneComponent* ParentComponent, TSharedPtr<FPrefabEditorTreeItem> ParentItem);

	/** Clears all actors from the preview scene world. */
	void ClearPreviewScene();

	/** The isolated preview environment */
	TSharedPtr<FAdvancedPreviewScene> PreviewScene;

	/** The viewport widget displaying the preview scene */
	TSharedPtr<SPrefabEditorViewport> ViewportWidget;

	/** The details view for editing selected actor properties */
	TSharedPtr<class IDetailsView> DetailsView;

	/** Hierarchy View (Tree of actors and components in the preview scene) */
	TSharedPtr<STreeView<TSharedPtr<FPrefabEditorTreeItem>>> HierarchyTreeView;
	TArray<TSharedPtr<FPrefabEditorTreeItem>> HierarchySource;

	TSharedRef<ITableRow> OnGenerateRowForHierarchy(TSharedPtr<FPrefabEditorTreeItem> InItem, const TSharedRef<STableViewBase>& OwnerTable);
	void OnHierarchySelectionChanged(TSharedPtr<FPrefabEditorTreeItem> InItem, ESelectInfo::Type SelectInfo);
	void OnGetChildrenForHierarchy(TSharedPtr<FPrefabEditorTreeItem> InItem, TArray<TSharedPtr<FPrefabEditorTreeItem>>& OutChildren);

	/** Recursively finds and selects an item in the hierarchy tree view. */
	bool FindAndSelectHierarchyItem_Recursive(UObject* ObjectToFind, const TArray<TSharedPtr<FPrefabEditorTreeItem>>& CurrentItems);

	/** Actors spawned for previewing a spawner's content. */
	TArray<TObjectPtr<AActor>> PreviewActors;
};
