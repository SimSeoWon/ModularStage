#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"
#include "UObject/WeakObjectPtr.h"

class IDetailLayoutBuilder;
class AActor;
class ABeacon;
class IPropertyHandle;

class FBeaconDetails : public IDetailCustomization
{
public:
	/** Makes a new instance of this detail layout class for a specific detail view requesting it */
	static TSharedRef<IDetailCustomization> MakeInstance();

	virtual ~FBeaconDetails();

	/** IDetailCustomization interface */
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;

private:
	TWeakObjectPtr<AActor> PreviewActorPtr;
	TSharedPtr<IPropertyHandle> PrefabAssetPathHandle;

	void UpdatePreviewInternal(ABeacon* BeaconActor); // Encapsulates preview logic
	void ClearExistingPreview(TWeakObjectPtr<ABeacon> Beacon); // Encapsulates preview clearing logic
};

