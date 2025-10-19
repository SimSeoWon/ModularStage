#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MissionPropComponent.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UMissionPropComponent : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MODULARSTAGE_API IMissionPropComponent
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void OnActivate() = 0;
	virtual void OnDeactivate() = 0;

};
