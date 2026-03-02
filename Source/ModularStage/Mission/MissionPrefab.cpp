// Fill out your copyright notice in the Description page of Project Settings.


#include "MissionPrefab.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/World.h"
#include "Component/HexGridVisualComponent.h"
#include "Component/MissionPropComponent.h"
#include "ModularStage/ModularStageDefine.h"

#if WITH_EDITOR
#include "Editor.h"
#include "Editor/EditorEngine.h"
#include "EditorUtilitySubsystem.h"
#include "ModularStageEditor/Public/Widget/UIListItem_InGameTask.h"
#endif

#include "ScopedTransaction.h"

#include "Component/ActorComponent_MissionTaskExecutor.h"

// Sets default values
AMissionPrefab::AMissionPrefab()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 프리팹의 위치, 회전, 크기를 담당할 루트 컴포넌트를 생성합니다.
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	HexGridVisualizer = CreateDefaultSubobject<UHexGridVisualComponent>(TEXT("HexGridVisualizer"));
	HexGridVisualizer->SetupAttachment(RootComponent);

	/// 해당 프리팹이 수행할 미션태스크를 실행하는 컴포넌트.
	MissionTaskExecutor = CreateDefaultSubobject<UActorComponent_MissionTaskExecutor>(TEXT("MissionTaskExecutor"));

    ObstacleExtent = FVector(100.f, 100.f, 100.f);

	bShowHexGridInGame = true;
}

void AMissionPrefab::PostInitProperties()
{
	Super::PostInitProperties();

	// 19개 타일 클러스터 초기화 (중심 + 2겹)
	if (SeptetTiles.Num() == 0)
	{
		auto AddTile = [this](FHexCoord Coord) {
			FVector Location = UHexGridLibrary::HexToWorld(Coord);
			SeptetTiles.Add(FHexTileConfig(Coord, EHexTileRole::Floor, Location));
			};

		// 1겹(7개): 중심(0,0) + 주변 6개
		AddTile(FHexCoord(0, 0));
		AddTile(FHexCoord(1, 0));   AddTile(FHexCoord(1, -1));
		AddTile(FHexCoord(0, -1));  AddTile(FHexCoord(-1, 0));
		AddTile(FHexCoord(-1, 1));  AddTile(FHexCoord(0, 1));

		// 2겹(12개 추가, 총 19개)
		AddTile(FHexCoord(2, 0));   AddTile(FHexCoord(2, -1));  AddTile(FHexCoord(2, -2));
		AddTile(FHexCoord(1, -2));  AddTile(FHexCoord(0, -2));  AddTile(FHexCoord(-1, -1));
		AddTile(FHexCoord(-2, 0));  AddTile(FHexCoord(-2, 1));  AddTile(FHexCoord(-2, 2));
		AddTile(FHexCoord(-1, 2));  AddTile(FHexCoord(0, 2));   AddTile(FHexCoord(1, 1));
	}
}

FVector AMissionPrefab::GetTileLocation(int32 Index) const
{
	if (SeptetTiles.IsValidIndex(Index))
	{
		// 계산하지 않고 저장된 중앙 위치 데이터를 즉시 반환합니다.
		return SeptetTiles[Index].CenterLocation;
	}
	return FVector::ZeroVector;
}

void AMissionPrefab::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	// 1. 프리팹 자체 그리드 스냅 기능 (에디터 전용)
#if WITH_EDITOR
	if (bSnapToHexGrid)
	{
		FHexCoord NearestHex = UHexGridLibrary::WorldToHex(GetActorLocation());
		FVector SnappedPos = UHexGridLibrary::HexToWorld(NearestHex);
		SnappedPos.Z = GetActorLocation().Z;

		if (!GetActorLocation().Equals(SnappedPos, 1.0f))
		{
			SetActorLocation(SnappedPos);
		}
	}

	// 2. 자식 프롭 컴포넌트들의 타일 기반 자동 정렬
	TArray<USceneComponent*> ChildComponents;
	RootComponent->GetChildrenComponents(true, ChildComponents);

	for (USceneComponent* Child : ChildComponents)
	{
		if (IMissionPropComponent* PropInterface = Cast<IMissionPropComponent>(Child))
		{
			int32 TileIdx = PropInterface->GetHexTileIndex();

			// 인덱스 범위 체크 (Septet: 0~6)
			if (SeptetTiles.IsValidIndex(TileIdx))
			{
				FVector TargetRelativePos = GetTileLocation(TileIdx);

				// 컴포넌트의 로컬 위치를 해당 타일 중심점으로 강제 고정
				if (!Child->GetRelativeLocation().Equals(TargetRelativePos, 0.1f))
				{
					Child->SetRelativeLocation(TargetRelativePos);
				}
			}
		}
	}
#endif

	if (HexGridVisualizer)
	{
		HexGridVisualizer->SetHiddenInGame(!bShowHexGridInGame);
		HexGridVisualizer->UpdateTileConfigs(SeptetTiles);
	}
}

// Called when the game starts or when spawned
void AMissionPrefab::BeginPlay()
{
	Super::BeginPlay();

	if (IsValid(MissionTaskExecutor))
	{
		const bool bIsReady = MissionTaskExecutor->SetData(TaskList);
		if (bIsReady)
		{
			MissionTaskExecutor->RegisterWithManager();
		}
	}
}

// Called every frame
void AMissionPrefab::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMissionPrefab::OpenMeshGenerator()
{
#if WITH_EDITOR
   
#endif
}

void AMissionPrefab::UnpackForEditing()
{
#if WITH_EDITOR
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
#endif
}

#if WITH_EDITOR
void AMissionPrefab::SetTaskList(const  TArray<UEntryData_InGameTaskList*>& inEntryDataList)
{
	TaskList.Reset(inEntryDataList.Num());

	for (auto iter : inEntryDataList)
	{
		if (false == IsValid(iter))
			continue;

		FMissionTaskInfo taskInfo;
		taskInfo.Title = iter->Title;
		taskInfo.Desc = iter->Desc;
		taskInfo.Step = iter->Step;
		taskInfo.Type = iter->Type;
		taskInfo.SerializeID = iter->SerializeID;
		taskInfo.ParentID = iter->ParentID;
		
		// Copy the instanced struct data
		taskInfo.TaskDetail = iter->TaskDetail;

		TaskList.Add(taskInfo);
	}
}
#endif