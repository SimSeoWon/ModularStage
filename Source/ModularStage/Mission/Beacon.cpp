#include "Beacon.h"
#include "AI/Navigation/NavigationTypes.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Engine/Blueprint.h"
#include "TimerManager.h"
#include "NavModifierVolume.h"
#include "NavAreas/NavArea_Null.h"
#include "Components/BrushComponent.h"
#include "Components/ChildActorComponent.h"
#include "Engine/World.h"



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
		// 부모(Beacon)가 Movable이면 자식도 Movable이어야 붙을 수 있습니다.
		if (USceneComponent* VolRoot = NavModifierVolume->GetRootComponent())
		{
			VolRoot->SetMobility(EComponentMobility::Movable);
		}

		NavModifierVolume->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
		NavModifierVolume->SetActorScale3D(VolumeSize / 100.f); // Assuming the default brush is 100x100x100
		NavModifierVolume->SetAreaClass(UNavArea_Null::StaticClass());

		UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
		if (NavSys)
		{
			if (UBrushComponent* BrushComp = NavModifierVolume->GetBrushComponent())
			{
				FBox Bounds = BrushComp->Bounds.GetBox();
				NavSys->AddDirtyArea(Bounds, ENavigationDirtyFlag::All);
			}
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

void ABeacon::OnSelectAssetPath(FString inPath)
{
	PrefabAssetPath = inPath;
	MarkPackageDirty();
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

void ABeacon::UpdatePreview()
{
#if WITH_EDITOR
	UWorld* World = GetWorld();
	if (!World || !World->IsEditorWorld())
	{
		return;
	}

	// If PrefabAssetPath is empty, destroy the preview component
	if (PrefabAssetPath.IsEmpty())
	{
		if (PreviewComp)
		{
			Modify();
			PreviewComp->DestroyComponent();
			PreviewComp = nullptr;
		}
		return;
	}


	Modify(); // 트랜잭션 기록 시작

	// 1. 기존 컴포넌트가 있다면 정리
	if (PreviewComp)
	{
		PreviewComp->DestroyComponent();
		PreviewComp = nullptr;
	}


	// Load the class from the path
	FSoftObjectPath assetPath(PrefabAssetPath);
	UObject* loadedAsset = assetPath.TryLoad();
	UBlueprint* blueprintAsset = Cast<UBlueprint>(loadedAsset);
	TSubclassOf<AActor> childClass = nullptr;
	if (blueprintAsset)
	{
		childClass = Cast<UClass>(blueprintAsset->GeneratedClass);
	}

	if (!childClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("UpdatePreview: Failed to load class from path: %s"), *PrefabAssetPath);
		if (PreviewComp)
		{
			PreviewComp->DestroyComponent();
			PreviewComp = nullptr;
		}
		return;
	}

	// 3. 컴포넌트 새로 생성
	// 이름 충돌 방지를 위해 MakeUniqueObjectName 사용 권장
	FName compName = MakeUniqueObjectName(this, UChildActorComponent::StaticClass(), TEXT("PreviewComponent"));
	PreviewComp = NewObject<UChildActorComponent>(this, compName);
	PreviewComp->CreationMethod = EComponentCreationMethod::Instance;
	PreviewComp->SetFlags(RF_Transient | RF_DuplicateTransient | RF_TextExportTransient);

	PreviewComp->SetFlags(RF_Transient | RF_TextExportTransient); // 객체 자체도 저장 안 함
	PreviewComp->SetupAttachment(RootComponent); // AttachToComponent보다 SetupAttachment 권장 (아직 등록 전이므로)
	PreviewComp->SetChildActorClass(childClass);
	PreviewComp->SetRelativeTransform(RelativeTransform);

	PreviewComp->RegisterComponent();

	if (AActor* ChildActor = PreviewComp->GetChildActor())
	{
		// Child Actor 자체도 에디터 전용으로 설정하여 2차 방지
		ChildActor->bIsEditorOnlyActor = true;;
		ChildActor->SetActorLabel(TEXT("PreviewActor_") + childClass->GetName()); // 아웃라이너 식별용 (선택)
	}
#endif
}


void ABeacon::ClearPreview()
{
#if WITH_EDITOR
	if (PreviewComp)
	{
		Modify();
		PreviewComp->DestroyComponent();
		PreviewComp = nullptr;
	}
#endif
}


