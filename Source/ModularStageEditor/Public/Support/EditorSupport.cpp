// Fill out your copyright notice in the Description page of Project Settings.

#include "EditorSupport.h"

#include "Engine/StaticMeshActor.h"
#include "Editor/UnrealEd/Public/EditorViewportClient.h"
#include "EditorUtilityBlueprint.h"
#include "EditorUtilitySubsystem.h"
#include "EditorUtilityWidgetBlueprint.h"
#include "Editor/EditorEngine.h"
#include "Editor.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HAL/PlatformApplicationMisc.h"

//#include "../AlphaEUW_HexTileInfo.h"
//#include "../EUW_InGameTask.h"

UEditorUtilityWidget* FEditorSupport::TryLoad(FString inPath, FName& inTabID)
{
	FSoftObjectPath ObjectPath;
	ObjectPath.SetPath(inPath);

	UObject* object = ObjectPath.TryLoad();

	if (false == IsValid(object))
		return nullptr;

	UEditorUtilityWidgetBlueprint* editorWidget = Cast<UEditorUtilityWidgetBlueprint>(object);
	if (false == IsValid(editorWidget))
		return nullptr;

	UEditorUtilitySubsystem* editorUtilitySubsystem = GEditor->GetEditorSubsystem<UEditorUtilitySubsystem>();
	if (false == IsValid(editorUtilitySubsystem))
		return nullptr;

	editorUtilitySubsystem->SpawnAndRegisterTabAndGetID(editorWidget, inTabID);

	return editorWidget->GetCreatedWidget();
}