// Copyright 2024. Cloner Co, Ltd. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_FireProjectile.generated.h"

class AProjectile;

/**
 * AnimNotify for firing a projectile from a character.
 */
UCLASS()
class MODULARSTAGE_API UAnimNotify_FireProjectile : public UAnimNotify
{
	GENERATED_BODY()

public:
	UAnimNotify_FireProjectile();

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	TSubclassOf<AProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	FName SocketName;
};
