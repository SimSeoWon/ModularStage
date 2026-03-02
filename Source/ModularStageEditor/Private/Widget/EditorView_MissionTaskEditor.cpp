// Fill out your copyright notice in the Description page of Project Settings.


#include "EditorView_MissionTaskEditor.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/ComboBoxString.h"
#include "Components/TextBlock.h"
#include "Components/PanelWidget.h"
#include "Components/WidgetSwitcher.h"
#include "UIListItem_InGameTask.h"
#include "EUW_InGameTaskEditor.h"
#include "Support/EditorSupport.h"
#include "ModularStage/Table/TableEnum.h"
#include "ModularStage/Table/MissionTaskTable.h"

void UEditorView_MissionTaskEditor::NativeConstruct()
{
	Super::NativeConstruct();

	OnRegisterFunctionMap();

	if (ComboBox_Type->OnSelectionChanged.IsBound())
	{
		ComboBox_Type->OnSelectionChanged.Clear();
	}
	ComboBox_Type->OnSelectionChanged.AddDynamic(this, &UEditorView_MissionTaskEditor::OnChanged_TaskType);

	const UEnum* ptrEnum = StaticEnum<EMissionObjectiveType>();
	if (ptrEnum)
	{
		ComboBox_Type->ClearOptions();
		int32 count = ptrEnum->NumEnums();
		FString strType = TEXT("None");
		for (int32 i = 0; i < count; i++)
		{
			strType = ptrEnum->GetNameStringByIndex(i);
			ComboBox_Type->AddOption(strType);
		}
	}
}
void UEditorView_MissionTaskEditor::NativeDestruct()
{
	Super::NativeDestruct();
}

void UEditorView_MissionTaskEditor::SetData(UEntryData_InGameTaskList* inEntryData)
{
	EntryData = inEntryData;
	if (false == IsValid(EntryData))
		return;

	if (EInGameTaskType::MISSION != EntryData->Type)
		return;

	// Ensure initialized as FMissionTaskData_Mission
	if (!EntryData->TaskDetail.IsValid() || EntryData->TaskDetail.GetScriptStruct() != FMissionTaskData_Mission::StaticStruct())
	{
		EntryData->TaskDetail.InitializeAs<FMissionTaskData_Mission>();
	}

	EMissionObjectiveType type = EMissionObjectiveType::None;
	FMissionTaskData_Mission* Data = EntryData->TaskDetail.GetMutablePtr<FMissionTaskData_Mission>();
	if (Data)
	{
		type = Data->ObjectiveType;
	}
	
	FString strType = TEXT("None");
	const UEnum* ptrEnum = StaticEnum<EMissionObjectiveType>();
	if (ptrEnum)
	{
		strType = ptrEnum->GetNameStringByValue((int64)type);
	}
	ComboBox_Type->SetSelectedOption(strType);

	SetDetail();
}

void UEditorView_MissionTaskEditor::OnChanged_TaskType(FString inSelectedItem, ESelectInfo::Type inSelectType)
{
	if (false == IsValid(EntryData))
		return;

	if (EInGameTaskType::MISSION != EntryData->Type)
		return;

	const UEnum* ptrEnum = StaticEnum<EMissionObjectiveType>();
	if (nullptr == ptrEnum)
		return;

	EMissionObjectiveType type = EMissionObjectiveType::None;
	FName typeName = FName(*ComboBox_Type->GetSelectedOption());
	int64 enumValue = ptrEnum->GetValueByName(typeName);
	if (INDEX_NONE == enumValue)
		return;

	type = StaticCast<EMissionObjectiveType>(enumValue);
	
	FMissionTaskData_Mission* Data = EntryData->TaskDetail.GetMutablePtr<FMissionTaskData_Mission>();
	if (Data)
	{
		Data->ObjectiveType = type;
	}

	SetDetail();
}

#pragma region Setting Detail widget
void UEditorView_MissionTaskEditor::OnRegisterFunctionMap()
{
	FunctionMap.Reset();
	const UEnum* ptrEnum = StaticEnum<EMissionObjectiveType>();
	if (nullptr == ptrEnum)
		return;

	int32 count = ptrEnum->NumEnums();
	for (int32 i = 0; i < count; i++)
	{
		int64 value = ptrEnum->GetValueByIndex(i);
		FString enum_to_name = ptrEnum->GetNameStringByValue(value);
		FString func_name = TEXT("SetDetail_") + enum_to_name;
		if (UFunction* func = GetClass()->FindFunctionByName(FName(*func_name)))
		{
			FunctionMap.Emplace(static_cast<EMissionObjectiveType>(value), func);
		}
	}
}

void UEditorView_MissionTaskEditor::SetDetail()
{
	Switcher_Detail->SetActiveWidget(Panel_Empty);
	if (false == IsValid(EntryData))
		return;

	if (EInGameTaskType::MISSION != EntryData->Type)
		return;

	EMissionObjectiveType type = EMissionObjectiveType::None;

	FMissionTaskData_Mission* Data = EntryData->TaskDetail.GetMutablePtr<FMissionTaskData_Mission>();
	if (Data)
	{
		type = Data->ObjectiveType;
	}

	if (false == FunctionMap.Contains(type))
		return;

	UFunction* func = FunctionMap[type];
	if (false == IsValid(func))
		return;

	this->ProcessEvent(func, nullptr);
}

void UEditorView_MissionTaskEditor::SetDetail_None()
{
	Switcher_Detail->SetActiveWidget(Panel_Empty);
}

void UEditorView_MissionTaskEditor::SetDetail_Destination()
{
	Switcher_Detail->SetActiveWidget(Panel_Destination);
}

void UEditorView_MissionTaskEditor::SetDetail_Annihilation()
{
	Switcher_Detail->SetActiveWidget(Panel_Annihilation);
}

#pragma endregion

