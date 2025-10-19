#include "EditorWidget_AssetSelector.h"
#include "ContentBrowserModule.h"
#include "IContentBrowserSingleton.h"
#include "AssetRegistry/AssetData.h"
#include "Components/ContentBrowserWrapper.h"
#include "EditorUtilityWidgetComponents.h"


void UEditorWidget_AssetSelector::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(ContentBrowser))
	{
		ContentBrowser.Get()->GetOnAssetSelected().BindUObject(this, &UEditorWidget_AssetSelector::OnAssetSelected);
	}

}

void UEditorWidget_AssetSelector::NativeDestruct()
{
	Super::NativeDestruct();
}

void UEditorWidget_AssetSelector::OnAssetSelected(const FAssetData& inAssetData) 
{
	Edit_Path->SetText(FText::FromString(inAssetData.GetObjectPathString()));
}

FString UEditorWidget_AssetSelector::GetSelectedAssetPath()
{
	// We get the selection directly from the Content Browser module,
	// as this is the most reliable way to get the user's selection in the editor.
	FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
	IContentBrowserSingleton& ContentBrowserSingleton = ContentBrowserModule.Get();

	TArray<FAssetData> SelectedAssets;
	ContentBrowserSingleton.GetSelectedAssets(SelectedAssets);

	if (SelectedAssets.Num() > 0)
	{
		// Return the path of the first selected asset.
		return SelectedAssets[0].GetObjectPathString();
	}

	return FString();
}
