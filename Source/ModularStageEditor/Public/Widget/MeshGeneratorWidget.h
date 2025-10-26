// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SWidget.h"
#include "PreviewScene.h"
#include "Widget/EditorWidgetBase.h"
#include "MeshGeneratorWidget.generated.h"

class AMissionPrefab;
class SEditorViewport;

/**
 * An Editor Utility Widget responsible for generating a static mesh for a MissionPrefab.
 */
UCLASS()
class MODULARSTAGEEDITOR_API UMeshGeneratorWidget : public UEditorWidgetBase
{
	GENERATED_BODY()

public:
	static FName GetMenuName() { return FName(TEXT("Mesh Generator Editor")); }
	static FText GetMenuDisplayName() { return FText::FromString(TEXT("Mesh Generator Editor")); }
	static FText GetMenuTooltip() { return FText::FromString(TEXT("Mesh Generator Editor")); }
	static FString GetWidgetBlueprintPath() { return TEXT("/Game/nevercook/editor/BP_MeshGeneratorEditor.BP_MeshGeneratorEditor"); }

public:
	UMeshGeneratorWidget();

	/** Sets the target prefab that this widget will operate on. */
	void SetTargetPrefab(AMissionPrefab* InPrefab);

	// UWidget interface
	virtual TSharedRef<SWidget> RebuildWidget() override;
	// End of UWidget interface

	// UObject interface
	virtual void BeginDestroy() override;
	// End of UObject interface

protected:
	/** The MissionPrefab actor that is being targeted. */
	UPROPERTY(BlueprintReadOnly, Category = "Mesh Generation")
	TWeakObjectPtr<AMissionPrefab> TargetPrefab;

	/** 
	 * This function is called from the Blueprint to trigger the mesh generation process.
	 * It reads properties from the TargetPrefab, generates a mesh, and saves it as a StaticMesh asset.
	 */
	UFUNCTION(BlueprintCallable, Category = "Mesh Generation")
	void GenerateMesh();

private:
	void SpawnTargetPrefab();
	
	/** The viewport widget */
	TSharedPtr<SEditorViewport> ViewportWidget;

	/** Level viewport client */
	TSharedPtr<class FEditorViewportClient> LevelViewportClient;

	/** Preview scene */
	TUniquePtr<FPreviewScene> PreviewScene;
};
