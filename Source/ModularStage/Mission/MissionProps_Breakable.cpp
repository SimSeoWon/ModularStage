#include "Mission/MissionProps_Breakable.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "GeometryCollection/GeometryCollection.h"
#include "Components/ShapeComponent.h"
#include "Net/UnrealNetwork.h"

AMissionProps_Breakable::AMissionProps_Breakable()
{
    bAlwaysRelevant = true;
    bReplicates = true;
    PrimaryActorTick.bCanEverTick = true;
    bIsBroken = false;

	// Create and set the root component
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);

	// Create the container for collision components
	CollisionParentComponent = CreateDefaultSubobject<USceneComponent>(TEXT("CollisionParent"));
	CollisionParentComponent->SetupAttachment(RootComponent);

	// Create the geometry collection component
	GeometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("GeometryCollection"));
	GeometryCollection->SetupAttachment(RootComponent);
    GeometryCollection->SetCollisionProfileName(TEXT("Destructible"));
    GeometryCollection->SetIsReplicated(false); // Chaos a tore des problemes de replication. On peut le laisser en false et le detruire/creer a la main
    GeometryCollection->ObjectType = EObjectStateTypeEnum::Chaos_Object_Kinematic;
    GeometryCollection->SetSimulatePhysics(false);
}

void AMissionProps_Breakable::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);
    DOREPLIFETIME(AMissionProps_Breakable, bIsBroken);
}

void AMissionProps_Breakable::BeginPlay()
{
	Super::BeginPlay();

    // Cache original state for reset
    if (GeometryCollection)
    {
        const UGeometryCollection* source = GeometryCollection->GetRestCollection();
        if (IsValid(source))
        {
            SourceAsset = const_cast<UGeometryCollection*>(source);
        }

        SavedTransform = GetActorTransform();
        if (GetAttachParentActor())
        {
            AttachSocketName = GetAttachParentSocketName();
        }
    }

	// Cache attached collision components
	if (CollisionParentComponent)
	{
		TArray<USceneComponent*> children;
		CollisionParentComponent->GetChildrenComponents(true, children);
		for (USceneComponent* child : children)
		{
			if (UShapeComponent* shape = Cast<UShapeComponent>(child))
			{
				ArrCollisionList.Add(shape);
			}
		}
	}
}

void AMissionProps_Breakable::OnHit_Implementation(const FHitData& HitData)
{
    if (bIsBroken)
    {
        return;
    }

    FVector HitLocation = HitData.Victim ? HitData.Victim->GetActorLocation() : GetActorLocation();
    FVector HitNormal = HitData.Direction;

    Multicast_OnBroken(HitLocation, HitNormal);
}

void AMissionProps_Breakable::Multicast_OnBroken_Implementation(const FVector& HitLocation, const FVector& HitNormal)
{
    if (bIsBroken)
    {
        return;
    }
    bIsBroken = true;

	// Logic to apply physics/destruction effect
    if (GeometryCollection)
    {
        GeometryCollection->SetSimulatePhysics(true);
    }
}

void AMissionProps_Breakable::ResetGeometryCollection()
{
    if (!bIsBroken || !SourceAsset)
    {
        return;
    }

    if (GeometryCollection)
    {
        GeometryCollection->DestroyComponent();
    }

    GeometryCollection = NewObject<UGeometryCollectionComponent>(this, TEXT("GeometryCollection"));
    if (GeometryCollection)
    {
        GeometryCollection->SetupAttachment(RootComponent);
        GeometryCollection->SetRestCollection(SourceAsset);
        GeometryCollection->SetCollisionProfileName(TEXT("Destructible"));
        GeometryCollection->SetIsReplicated(false);
        GeometryCollection->ObjectType = EObjectStateTypeEnum::Chaos_Object_Kinematic;
        GeometryCollection->SetSimulatePhysics(false);
        GeometryCollection->RegisterComponent();
    }

    if (GetAttachParentActor())
    {
        AttachToActor(GetAttachParentActor(), FAttachmentTransformRules::KeepWorldTransform, AttachSocketName);
    }
    SetActorTransform(SavedTransform, false, nullptr, ETeleportType::TeleportPhysics);

    bIsBroken = false;
}