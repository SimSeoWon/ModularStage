#include "Beacon.h"
#include "AI/Navigation/NavigationTypes.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "TimerManager.h"
#include "NavModifierVolume.h"
#include "NavAreas/NavArea_Null.h"
#include "Components/BrushComponent.h"

// Sets default values
ABeacon::ABeacon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = StaticMeshComponent;

	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	StaticMeshComponent->SetCollisionResponseToAllChannels(ECR_Block);
}

// Called when the game starts or when spawned
void ABeacon::BeginPlay()
{
	Super::BeginPlay();

	// Spawn a NavModifierVolume to prevent navmesh generation in the area
	ANavModifierVolume* NavModifierVolume = GetWorld()->SpawnActor<ANavModifierVolume>(GetActorLocation(), GetActorRotation());
	if (NavModifierVolume)
	{
		NavModifierVolume->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
		NavModifierVolume->SetActorScale3D(VolumeSize / 100.f); // Assuming the default brush is 100x100x100
		NavModifierVolume->SetAreaClass(UNavArea_Null::StaticClass());

		UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
		if (NavSys)
		{
			FBox Bounds = NavModifierVolume->GetBrushComponent()->Bounds.GetBox();
			NavSys->AddDirtyArea(Bounds, ENavigationDirtyFlag::All);
		}
	}

	LoadAndSpawnMesh();
}

// Called every frame
void ABeacon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABeacon::LoadAndSpawnMesh()
{
	if (MeshToLoad.IsValid())
	{
		FStreamableManager& StreamableManager = UAssetManager::Get().GetStreamableManager();
		StreamableManager.RequestAsyncLoad(MeshToLoad, FStreamableDelegate::CreateUObject(this, &ABeacon::OnMeshLoaded));
	}
}

void ABeacon::OnMeshLoaded()
{
	UStaticMesh* LoadedMesh = Cast<UStaticMesh>(MeshToLoad.ResolveObject());
	if (!LoadedMesh)
	{
		UE_LOG(LogTemp, Error, TEXT("ABeacon: Failed to load mesh from %s"), *MeshToLoad.ToString());
		return;
	}

	if (StaticMeshComponent)
	{
		StaticMeshComponent->SetStaticMesh(LoadedMesh);
		StaticMeshComponent->SetVisibility(false);
		StaticMeshComponent->SetHiddenInGame(true);
		// Ensure collision is enabled for NavMesh generation
		StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		StaticMeshComponent->SetCollisionResponseToAllChannels(ECR_Block);

		UWorld* World = GetWorld();
		UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(World);

		if (NavSys && World)
		{
			// Delay the NavMesh update slightly to ensure the component's transform and bounds are fully updated.
			World->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateLambda([this, NavSys]()
			{
				if (StaticMeshComponent)
				{
					const FBox Bounds = StaticMeshComponent->Bounds.GetBox();
					NavSys->AddDirtyArea(Bounds, ENavigationDirtyFlag::All);
					UE_LOG(LogTemp, Warning, TEXT("ABeacon: NavMesh updated for bounds: %s"), *Bounds.ToString());
				}
			}));
		}
	}
}