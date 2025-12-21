#pragma once

#include "CoreMinimal.h"
#include "EditorWidgetBase.h"
#include "EditorWidget_AssetSelector.generated.h"

class UEditorUtilityButton;
class UContentBrowserWrapper;
class UEditorUtilityEditableTextBox;
struct FAssetData;

/**
 * An editor widget that displays a list of assets and allows retrieving the path of a selected asset.
 * It is designed to be subclassed in Blueprints. In the UMG editor of the child Blueprint,
 * a 'Content Browser' widget should be added and bound to the 'ContentBrowser' property.
 */
UCLASS()
class MODULARSTAGEEDITOR_API UEditorWidget_AssetSelector : public UEditorWidgetBase
{
	GENERATED_BODY()

public:
	// Default implementation for the widget blueprint path. Child classes should hide this.
	static FString GetWidgetBlueprintPath()
	{
		return TEXT("/Game/nevercook/editor/menu/BP_EditorWidget_AssetSelector.BP_EditorWidget_AssetSelector");
	}

	DECLARE_DELEGATE_OneParam(OnAcceptAssetPathDelegate, FString)
	OnAcceptAssetPathDelegate& GetOnAcceptAssetPath() { return OnAcceptAssetPath; }

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION()
	void OnClicked_Accept();

	UFUNCTION()
	void OnAssetSelected(const FAssetData& inAssetData);

protected:
	UPROPERTY(meta = (BindWidget))
	UEditorUtilityButton* Btn_Accept = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Content Browser", meta = (BindWidget))
	TObjectPtr<UEditorUtilityEditableTextBox> Edit_Path;

	UPROPERTY(BlueprintReadOnly, Category = "Content Browser", meta = (BindWidget))
	TObjectPtr<UContentBrowserWrapper> ContentBrowser;

	UFUNCTION(BlueprintCallable, Category = "Content Browser")
	FString GetSelectedAssetPath();


	OnAcceptAssetPathDelegate OnAcceptAssetPath;
};
