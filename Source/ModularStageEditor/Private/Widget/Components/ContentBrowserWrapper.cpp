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

    // �� UMG ������ �Ҹ�� ��, �츮�� ������ �ִ� SAssetView�� ����
    // TSharedPtr ������ ��������� �����Ͽ� �޸𸮿��� �����ǵ��� �մϴ�.
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
