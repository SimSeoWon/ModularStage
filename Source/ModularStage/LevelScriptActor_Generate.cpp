// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelScriptActor_Generate.h"
#include "EngineUtils.h"
#include "Engine/Blueprint.h"
#include "BlueprintEditorLibrary.h"
#include "ModularStage/Mission/MissionPrefab.h"


// Sets default values
ALevelScriptActor_Generate::ALevelScriptActor_Generate()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ALevelScriptActor_Generate::BeginPlay()
{
	Super::BeginPlay();
}

void ALevelScriptActor_Generate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

//#if WITH_EDITOR
//void ALevelScriptActor_Generate::FocusEditorCameraOnActors(const TArray<AActor*>& inActors)
//{
//	if (inActors.Num() <= 0) return;
//
//	FBox BoundingBox(ForceInit);
//	for (AActor* Actor : inActors)
//	{
//		if (Actor) BoundingBox += Actor->GetComponentsBoundingBox(true);
//	}
//
//	if (!IsValid(GEditor)) return;
//
//	FViewport* viewport = GEditor->GetActiveViewport();
//	if (!viewport) return;
//
//	FEditorViewportClient* editorViewport = (FEditorViewportClient*)viewport->GetClient();
//	if (!editorViewport) return;
//
//	editorViewport->FocusViewportOnBox(BoundingBox);
//}
//#endif