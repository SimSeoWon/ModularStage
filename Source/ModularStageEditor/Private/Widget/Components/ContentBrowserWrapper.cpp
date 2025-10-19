#include "ContentBrowserWrapper.h"
#include "Engine/BlueprintGeneratedClass.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Kismet2/KismetEditorUtilities.h"

#include "ContentBrowserDelegates.h"
#include "Modules/ModuleManager.h"
#include "ContentBrowserModule.h"
#include "IContentBrowserSingleton.h"
#include "Editor.h"

void UContentBrowserWrapper::ReleaseSlateResources(bool bReleaseChildren)
{
    Super::ReleaseSlateResources(bReleaseChildren);

    // 이 UMG 위젯이 소멸될 때, 우리가 가지고 있던 SAssetView에 대한
    // TSharedPtr 참조를 명시적으로 리셋하여 메모리에서 해제되도록 합니다.
    AssetPickerWidget.Reset();
}

TSharedRef<SWidget> UContentBrowserWrapper::RebuildWidget()
{
    FContentBrowserModule& module = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");

    FAssetPickerConfig AssetPickerConfig;
    AssetPickerConfig.OnAssetSelected = FOnAssetSelected::CreateUObject(this, &UContentBrowserWrapper::HandleAssetSelected);
    // We are just creating the picker, not showing it in a window.
    AssetPickerWidget = module.Get().CreateAssetPicker(AssetPickerConfig);

    return AssetPickerWidget.ToSharedRef();
}

void UContentBrowserWrapper::HandleAssetSelected(const FAssetData& AssetData)
{
    if (false == OnAssetSelected.IsBound())
        return;

    OnAssetSelected.Execute(AssetData);
}
