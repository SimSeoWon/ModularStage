#pragma once

#include "CoreMinimal.h"
#include "Components/Widget.h"
#include "ContentBrowserWrapper.generated.h"

/**
 * A UMG wrapper for the SAssetPicker slate widget.
 */
UCLASS(meta = (DisplayName = "Content Browser", Category = "Modular Stage"))
class MODULARSTAGEEDITOR_API UContentBrowserWrapper : public UWidget
{
    GENERATED_BODY()

public:
    //~ Begin UWidget Interface
    virtual TSharedRef<SWidget> RebuildWidget() override;
    //~ End UWidget Interface

    virtual void ReleaseSlateResources(bool bReleaseChildren) override;

private:
    void HandleAssetSelected(const FAssetData& AssetData);

protected:
    TSharedPtr<SWidget> AssetPickerWidget;
};
