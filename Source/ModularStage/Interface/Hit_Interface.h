// Copyright 2024. Cloner Co, Ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interface/HitData.h"
#include "Hit_Interface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UHit_Interface : public UInterface
{
	GENERATED_BODY()
};

class MODULARSTAGE_API IHit_Interface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Hit")
	void OnHit(const FHitData& HitData);
};
