// Copyright 2024. Cloner Co, Ltd. All rights reserved.

#include "Character/AnimNotify/AnimNotify_FireProjectile.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"
#include "Projectile/Projectile.h"

UAnimNotify_FireProjectile::UAnimNotify_FireProjectile()
{
	SocketName = TEXT("Muzzle_01"); // Default socket name
}

void UAnimNotify_FireProjectile::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!ProjectileClass || !MeshComp || !MeshComp->GetOwner())
	{
		return;
	}

	AActor* OwnerActor = MeshComp->GetOwner();
	UWorld* World = OwnerActor->GetWorld();
	if (!World)
	{
		return;
	}

	FVector SpawnLocation = MeshComp->GetSocketLocation(SocketName);
	FRotator SpawnRotation = MeshComp->GetSocketRotation(SocketName);

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = OwnerActor;
	SpawnParams.Instigator = OwnerActor->GetInstigator(); // Assumes the owner is a pawn and has an instigator
    if (!SpawnParams.Instigator)
    {
        SpawnParams.Instigator = Cast<APawn>(OwnerActor); // Fallback for character itself
    }


	AProjectile* Projectile = World->SpawnActor<AProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
    // Optional: Add any post-spawn logic for the projectile if needed
}
