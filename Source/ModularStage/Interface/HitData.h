// Copyright 2024. Cloner Co, Ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "HitData.generated.h"

USTRUCT(BlueprintType)
struct FHitData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitData")
	TObjectPtr<AActor> Attacker;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitData")
	TObjectPtr<AActor> Victim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitData")
	FVector Direction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HitData")
	float Damage;
};
