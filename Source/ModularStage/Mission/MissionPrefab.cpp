// Fill out your copyright notice in the Description page of Project Settings.


#include "MissionPrefab.h"
#include "Components/SceneComponent.h"
#include "Engine/World.h"
#include "Editor.h"
#include "Editor/EditorEngine.h"
#include "ScopedTransaction.h"

#include "Component/ActorComponent_MissionTaskExecutor.h"

// Sets default values
AMissionPrefab::AMissionPrefab()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 프리팹의 위치, 회전, 크기를 담당할 루트 컴포넌트를 생성합니다.
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	/// 해당 프리팹이 수행할 미션태스크를 실행하는 컴포넌트.
	MissionTaskExecutor = CreateDefaultSubobject<UActorComponent_MissionTaskExecutor>(TEXT("MissionTaskExecutor"));
}

// Called when the game starts or when spawned
void AMissionPrefab::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMissionPrefab::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMissionPrefab::UnpackForEditing()
{
	UWorld* World = GetWorld();
	if (!World || World->WorldType != EWorldType::Editor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Unpacking is only available in the editor."));
		return;
	}

	const FScopedTransaction Transaction(FText::FromString("Unpack Prefab"));

	TArray<UChildActorComponent*> ChildActorComponents;
	GetComponents<UChildActorComponent>(ChildActorComponents);

	TArray<AActor*> SpawnedActors;

	UE_LOG(LogTemp, Log, TEXT("Unpacking %d child actors from prefab %s"), ChildActorComponents.Num(), *GetName());

	for (UChildActorComponent* Comp : ChildActorComponents)
	{
		if (!Comp) continue;

		UClass* ActorClass = Comp->GetChildActorClass();
		if (!ActorClass) continue;

		FTransform SpawnTransform = Comp->GetComponentTransform();

		AActor* NewActor = GEditor->AddActor(World->GetCurrentLevel(), ActorClass, SpawnTransform);
		if (NewActor)
		{
			SpawnedActors.Add(NewActor);
			UE_LOG(LogTemp, Log, TEXT("Spawned: %s"), *NewActor->GetName());
		}
	}

	if (SpawnedActors.Num() > 0)
	{
		World->EditorDestroyActor(this, false);
	}
}
