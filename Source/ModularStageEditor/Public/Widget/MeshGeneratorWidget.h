// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorWidgetBase.h"
#include "Editor/Blutility/Classes/EditorUtilityWidget.h"
#include "MeshGeneratorWidget.generated.h"

class SPreviewViewport;
class AMissionPrefab;
class FAdvancedPreviewScene;
class AActor; // Forward declare AActor

class USquareTileDataBase;
class USquareTileView;

/**
 * An Editor Utility Widget for generating meshes and previewing them.
 */
UCLASS()
class MODULARSTAGEEDITOR_API UMeshGeneratorWidget : public UEditorWidgetBase
{
	GENERATED_BODY()

public:
	static FName GetMenuName() { return FName(TEXT("Mesh Generator Editor")); }
	static FText GetMenuDisplayName() { return FText::FromString(TEXT("Mesh Generator Editor")); }
	static FText GetMenuTooltip() { return FText::FromString(TEXT("Mesh Generator Editor")); }

	static FString GetWidgetBlueprintPath() { return TEXT("/Game/nevercook/editor/BP_MeshGeneratorWidget.BP_MeshGeneratorWidget"); }

public:
	/** Sets the target prefab that this widget will operate on. */
	UFUNCTION(BlueprintCallable, Category = "Mesh Generation")
	void SetTargetPrefab(AMissionPrefab* InPrefab);

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

protected:
	UPROPERTY(EditAnywhere)
	int32 NumSquares = 1;
	UPROPERTY(EditAnywhere)
	float SquareSize = 100.0f;
	UPROPERTY(EditAnywhere)
	float UIScaleFactor = 0.1f;

	/** Path to the MissionPrefab asset to load. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh Generation")
	FString PrefabPath;

	FReply OnGenerateMeshClicked();
	FReply OnExportMeshClicked();
	FReply OnLoadPrefabClicked();
	FReply OnBrowsePrefabPathClicked();
	void OnTileToggled(USquareTileDataBase* TileData);

	/** The MissionPrefab actor that is being targeted. */
	UPROPERTY(BlueprintReadOnly, Category = "Mesh Generation")
	TWeakObjectPtr<AMissionPrefab> TargetPrefab;

	/** The actor being previewed in the scene. */
	UPROPERTY()
	TWeakObjectPtr<AActor> PreviewActor;

	/** The viewport widget */
	TSharedPtr<SPreviewViewport> PreviewViewportWidget;

	TSharedPtr<FAdvancedPreviewScene> PreviewScene;

	UPROPERTY(EditAnywhere, Category = "Mesh Generation")
	TSubclassOf<USquareTileView> SquareTileViewClass;

	UPROPERTY()
	USquareTileView* SquareTileView;
};