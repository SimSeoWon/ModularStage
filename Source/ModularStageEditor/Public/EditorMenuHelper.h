// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Misc/Attribute.h"
#include "Textures/SlateIcon.h"
#include "ToolMenuDelegates.h"
#include "Editor/UnrealEdTypes.h"

class FUICommandList;
class AActor;
/**
 *
 */

class FEditorMenuHelper
{
public:
	void InitMenu();
	void AddSheerToolMenuEntry(const FName section_name, const FName menu_name, const TAttribute<FText>& display_name, const TAttribute<FText>& tool_tip, const TAttribute<FSlateIcon> icon, const FToolUIActionChoice& action);
public:
	static FName base_menu_name;

};