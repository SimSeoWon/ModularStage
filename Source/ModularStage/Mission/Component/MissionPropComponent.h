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
	virtual void SetGuid(const FGuid& InGuid) = 0;
	virtual FGuid GetGuid() const = 0;
	virtual void OnActivate() = 0;
	virtual void OnDeactivate() = 0;

	/** 육각형 타일 인덱스 (0~6) 설정 및 반환 */
	virtual void SetHexTileIndex(int32 InIndex) = 0;
	virtual int32 GetHexTileIndex() const = 0;

};
