#pragma once

#include "CoreMinimal.h"
#include "Widget/EditorWidgetBase.h"
#include "StagePresetEditor.generated.h"

class UEditorUtilityButton;
class UEditorUtilityEditableText;
class ALevelScriptActor_Generate;
/**
 *
 */
UCLASS()
class MODULARSTAGEEDITOR_API UStagePresetEditor : public UEditorWidgetBase
{
	GENERATED_BODY()

public:
	static FName GetMenuName() { return FName(TEXT("Modular Stage Editor")); }
	static FText GetMenuDisplayName() { return FText::FromString(TEXT("Modular Stage Editor")); }
	static FText GetMenuTooltip() { return FText::FromString(TEXT("Modular Stage Editor")); }

	static FString GetWidgetBlueprintPath() { return TEXT("/Game/nevercook/editor/BP_StagePresetEditor.BP_StagePresetEditor"); }

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnClicked_Prefab();

	UFUNCTION()
	void OnClicked_Prefab_Components();

	void GetSelectedActors(TArray<AActor*>& OutActors);

	void CreatePrefabFromActors(const TArray<AActor*>& inActors, const FString& inSavePath);

	void CreatePrefabFromCollectedComponents(const TArray<AActor*>& InActors, const FString& inSavePath);

protected:
	UPROPERTY(meta = (BindWidget))
	UEditorUtilityEditableText* Edit_PackagePath = nullptr;

	UPROPERTY(meta = (BindWidget))
	UEditorUtilityButton* Btn_CreatePrefab = nullptr;
};