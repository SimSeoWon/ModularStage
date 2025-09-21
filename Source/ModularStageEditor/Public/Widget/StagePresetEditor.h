#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "StagePresetEditor.generated.h"

class UEditorUtilityButton;
class UEditorUtilityEditableText;
class ALevelScriptActor_Generate;
/**
 *
 */
UCLASS()
class MODULARSTAGEEDITOR_API UStagePresetEditor : public UEditorUtilityWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	void OnClicked_Prefab();

	UFUNCTION()
	void OnClicked_Prefab_Components();

	ALevelScriptActor_Generate* GetLevelScriptActor();

protected:
	UPROPERTY(meta = (BindWidget))
	UEditorUtilityEditableText* Edit_FolderName = nullptr;

	UPROPERTY(meta = (BindWidget))
	UEditorUtilityEditableText* Edit_PackagePath = nullptr;

	UPROPERTY(meta = (BindWidget))
	UEditorUtilityButton* Btn_CreatePrefab = nullptr;

	UPROPERTY(meta = (BindWidget))
	UEditorUtilityButton* Btn_CreatePrefab2 = nullptr;


};
