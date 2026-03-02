// Copyright 2024. Cloner Co, Ltd. All rights reserved.

#include "Projectile/Projectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "UObject/UObjectGlobals.h"
#include "Interface/Hit_Interface.h"
#include "Interface/HitData.h"

AProjectile::AProjectile()
{
 	PrimaryActorTick.bCanEverTick = false;

    // Create and configure the collision sphere
    CollisionShape = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
    CollisionShape->InitSphereRadius(20.0f);
    CollisionShape->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
    CollisionShape->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    RootComponent = CollisionShape;

    // Create and configure the projectile movement component
    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMoveComp"));
    ProjectileMovement->UpdatedComponent = CollisionShape;
    ProjectileMovement->InitialSpeed = 3000.f;
    ProjectileMovement->MaxSpeed = 3000.f;
    ProjectileMovement->bRotationFollowsVelocity = true;
    ProjectileMovement->bShouldBounce = false;
    ProjectileMovement->ProjectileGravityScale = 0.f; // Typically projectiles don't have gravity
}

void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
    // Bind the hit event
    CollisionShape->OnComponentHit.AddDynamic(this, &AProjectile::OnProjectileHit);
}

void AProjectile::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (!OtherActor || OtherActor == this || OtherActor == GetInstigator())
    {
        return;
    }

    // Check if the hit actor implements the hit interface
    if (OtherActor->GetClass()->ImplementsInterface(UHit_Interface::StaticClass()))
    {
        // Populate hit data
        FHitData HitData;
        HitData.Attacker = GetInstigator();
        HitData.Victim = OtherActor;
        HitData.Direction = GetActorForwardVector();
        HitData.Damage = Damage;

        // Execute the OnHit event on the interface
        IHit_Interface::Execute_OnHit(OtherActor, HitData);
    }

    // Destroy the projectile after impact
    Destroy();
}
