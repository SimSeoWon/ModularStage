#pragma once

#include "CoreMinimal.h"
#include "Commandlets/Commandlet.h"
#include "ExportBeaconLocationsCommandlet.generated.h"

UCLASS()
class UExportBeaconLocationsCommandlet : public UCommandlet
{
	GENERATED_BODY()

public:
	UExportBeaconLocationsCommandlet();

	//~ Begin UCommandlet Interface
	virtual int32 Main(const FString& Params) override;
	//~ End UCommandlet Interface
};
