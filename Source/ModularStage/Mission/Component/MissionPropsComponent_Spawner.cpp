// Copyright Epic Games, Inc. All Rights Reserved.

#include "MissionPropsComponent_Spawner.h"
#include "Engine/CollisionProfile.h"

#if WITH_EDITOR
#include "ModularStageEditor/Public/Widget/EditorWidget_AssetSelector.h"
#include "ModularStageEditor/Public/Widget/EditorWidgetBase.h"
#endif

UMissionPropsComponent_Spawner::UMissionPropsComponent_Spawner()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UMissionPropsComponent_Spawner::BeginPlay()
{
	Super::BeginPlay();
}

void UMissionPropsComponent_Spawner::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UMissionPropsComponent_Spawner::OnActivate()
{
	// Implementation
}

void UMissionPropsComponent_Spawner::OnDeactivate()
{
	// Implementation
}

void UMissionPropsComponent_Spawner::ShowAsserBrowser()
{
#if WITH_EDITOR
	UEditorWidget_AssetSelector* editor = UEditorWidgetBase::ShowEditorUtilityWidget<UEditorWidget_AssetSelector>();
	if (false == IsValid(editor))
		return;

	editor->Run();
#endif
}