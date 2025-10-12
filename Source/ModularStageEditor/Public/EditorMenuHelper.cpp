// Fill out your copyright notice in the Description page of Project Settings.

#include "EditorMenuHelper.h"
#include "ToolMenu.h"
#include "ToolMenus.h"

FName FEditorMenuHelper::base_menu_name = FName("LevelEditor.MainMenu.ModularStage");

void FEditorMenuHelper::InitMenu()
{
	UToolMenus* toolMenus = UToolMenus::Get();
	if (false == IsValid(toolMenus))
		return;

	UToolMenu* menu = toolMenus->FindMenu(base_menu_name); //
	if (false == IsValid(menu))
	{
		menu = toolMenus->ExtendMenu("LevelEditor.MainMenu");
		FToolMenuSection& section = menu->FindOrAddSection(NAME_None);
		FText label = FText::FromString(TEXT("ModularStage"));
		FText tooltip = FText::FromString(TEXT("ModularStage"));
		FToolMenuEntry& entry = section.AddSubMenu("ModularStage", label, tooltip, FNewToolMenuChoice());
		entry.InsertPosition = FToolMenuInsert("Help", EToolMenuInsertType::After);
		menu = toolMenus->RegisterMenu(base_menu_name);
	}
}


void FEditorMenuHelper::AddSheerToolMenuEntry(const FName section_name, const FName menu_name, const TAttribute<FText>& display_name, const TAttribute<FText>& tool_tip, const TAttribute<FSlateIcon> icon, const FToolUIActionChoice& action)
{
	UToolMenus* toolMenus = UToolMenus::Get();
	if (false == IsValid(toolMenus))
		return;

	UToolMenu* menu = toolMenus->FindMenu(base_menu_name); //
	if (false == IsValid(menu))
		return;

	FToolMenuSection* section = menu->FindSection(section_name);
	if (!section)
	{
		FToolMenuSection& new_section = menu->AddSection(section_name);
		new_section.AddMenuEntry(menu_name, display_name, tool_tip, icon, action);
	}
	else
	{
		section->AddMenuEntry(menu_name, display_name, tool_tip, icon, action);
	}
}