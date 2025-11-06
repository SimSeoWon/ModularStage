// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorWidgetBase.h"
#include "Editor/Blutility/Classes/EditorUtilityWidget.h"
#include "MeshGeneratorWidget.generated.h"

class SPreviewViewport;
class AMissionPrefab;

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

	/** The MissionPrefab actor that is being targeted. */
	UPROPERTY(BlueprintReadOnly, Category = "Mesh Generation")
	TWeakObjectPtr<AMissionPrefab> TargetPrefab;

	/** The viewport widget */
	TSharedPtr<SPreviewViewport> PreviewViewportWidget;
};