// Fill out your copyright notice in the Description page of Project Settings.


#include "EditorView_InGameTaskEditor.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/ComboBoxString.h"
#include "Components/TextBlock.h"
#include "Components/PanelWidget.h"
#include "Components/WidgetSwitcher.h"

#include "UIListItem_InGameTask.h"
#include "EditorView_MissionTaskEditor.h"
#include "EditorView_SpawnMonsterTaskEditor.h"
#include "EUW_InGameTaskEditor.h"
#include "Support/EditorSupport.h"

#include "ModularStage/Table/TableEnum.h"
#include "ModularStage/Table/MissionTaskTable.h"

void UEditorView_InGameTaskEditor::NativeConstruct()
{
	Super::NativeConstruct();
	OnRegisterFunctionMap();

	if (false == Btn_Save->OnClicked.IsBound())
	{
		Btn_Save->OnClicked.AddDynamic(this, &UEditorView_InGameTaskEditor::OnClicked_Save);
	}

	if (false == Btn_Close->OnClicked.IsBound())
	{
		Btn_Close->OnClicked.AddDynamic(this, &UEditorView_InGameTaskEditor::OnClicked_Close);
	}

	if (ComboBox_Type->OnSelectionChanged.IsBound())
	{
		ComboBox_Type->OnSelectionChanged.Clear();
	}
	ComboBox_Type->OnSelectionChanged.AddDynamic(this, &UEditorView_InGameTaskEditor::OnChanged_TaskType);

	const UEnum* ptrEnum = StaticEnum<EInGameTaskType>();
	if (ptrEnum)
	{
		ComboBox_Type->ClearOptions();
		int32 count = ptrEnum->NumEnums();
		FString strType = TEXT("NONE");
		for (int32 i = 0; i < count; i++)
		{
			strType = ptrEnum->GetNameStringByIndex(i);
			ComboBox_Type->AddOption(strType);
		}
	}

}
void UEditorView_InGameTaskEditor::NativeDestruct()
{

}



#pragma region Setting Detail widget 

void UEditorView_InGameTaskEditor::OnRegisterFunctionMap()
{
	FunctionMap.Reset();
	const UEnum* ptrEnum = StaticEnum<EInGameTaskType>();
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
			FunctionMap.Emplace(static_cast<EInGameTaskType>(value), func);
		}
	}
}

void UEditorView_InGameTaskEditor::SetDetail()
{
	Switcher_Detail->SetActiveWidget(Panel_Empty);
	if (false == IsValid(EntryData))
		return;

	EInGameTaskType type = EntryData->Type;
	if (false == FunctionMap.Contains(type))
		type = EInGameTaskType::NONE;

	if (false == FunctionMap.Contains(type))
		return;

	UFunction* func = FunctionMap[type];
	if (false == IsValid(func))
		return;

	this->ProcessEvent(func, nullptr);
}

void UEditorView_InGameTaskEditor::SetDetail_NONE()
{
	Switcher_Detail->SetActiveWidget(Panel_Empty);
}


void UEditorView_InGameTaskEditor::SetDetail_MISSION()
{
	Switcher_Detail->SetActiveWidget(Panel_Mission);
	if (false == IsValid(EntryData))
		return;

	// For MISSION type, we might use FMissionTaskData_Base or a specific multi-task data struct if added later.
	// Currently it delegates to MissionTaskEditor.
	if (IsValid(MissionTaskEditor))
	{
		MissionTaskEditor->SetData(EntryData);
	}
}

void UEditorView_InGameTaskEditor::SetDetail_SPAWN_OBJECT()
{
	Switcher_Detail->SetActiveWidget(Panel_SpawnObject);
	if (false == IsValid(EntryData))
		return;

	// Ensure the TaskDetail is initialized as FMissionTaskData_Object
	if (!EntryData->TaskDetail.IsValid() || EntryData->TaskDetail.GetScriptStruct() != FMissionTaskData_Object::StaticStruct())
	{
		EntryData->TaskDetail.InitializeAs<FMissionTaskData_Object>();
	}

	FMissionTaskData_Object* Data = EntryData->TaskDetail.GetMutablePtr<FMissionTaskData_Object>();
	if (Data)
	{
		// Update UI with Data->ActorID (Assuming there's a UI element for this, using Txt_SpawnActorID as placeholder if needed or just logging)
		// For now, let's just ensure the data exists.
	}
}

void UEditorView_InGameTaskEditor::SetDetail_SPAWN_MONSTER()
{
	Switcher_Detail->SetActiveWidget(Panel_SpawnMonster);
	if (false == IsValid(EntryData))
		return;

	//SpawnMonster->SetData(EntryData);

	// Ensure the TaskDetail is initialized as FMissionTaskData_Spawn
	//if (!EntryData->TaskDetail.IsValid() || EntryData->TaskDetail.GetScriptStruct() != FMissionTaskData_Spawn::StaticStruct())
	//{
	//	EntryData->TaskDetail.InitializeAs<FMissionTaskData_Spawn>();
	//}

	/*FMissionTaskData_Spawn* Data = EntryData->TaskDetail.GetMutablePtr<FMissionTaskData_Spawn>();
	if (Data)
	{
		Txt_SpawnActorID->SetText(FText::AsNumber(Data->ActorID));
		Txt_MonsterID->SetText(FText::AsNumber(Data->MonsterID));
	}*/
}
#pragma endregion

void UEditorView_InGameTaskEditor::OnSelected_HexagonTile(const FVector2D inPos/**= FVector2D::Zero()*/)
{

}

void UEditorView_InGameTaskEditor::SetData(UEntryData_InGameTaskList* inEntryData)
{
	if (false == IsValid(inEntryData))
		return;

	EntryData = inEntryData;
	TextBox_Step->SetText(FText::AsNumber(EntryData->Step));
	TextBox_Title->SetText(FText::FromString(EntryData->Title));
	TextBox_Desc->SetText(FText::FromString(EntryData->Desc));

	FString strType = TEXT("NONE");
	const UEnum* ptrEnum = StaticEnum<EInGameTaskType>();
	if (ptrEnum)
	{
		strType = ptrEnum->GetNameStringByValue((int64)EntryData->Type);
	}
	ComboBox_Type->SetSelectedOption(strType);

	SetDetail();
}

void UEditorView_InGameTaskEditor::OnClicked_Btn_ActorList()
{
	if (false == IsValid(EntryData))
		return;

	const FString DeleteInactiveEmitterUIPath = TEXT("EditorUtilityWidgetBlueprint'/Game/nevercook/editor/menu/bp_alphaeuw_actorlist.bp_alphaeuw_actorlist'");
	FName tabID;
}

void UEditorView_InGameTaskEditor::OnClicked_MonsterList()
{
	if (false == IsValid(EntryData))
		return;

	const FString DeleteInactiveEmitterUIPath = TEXT("EditorUtilityWidgetBlueprint'/Game/nevercook/editor/menu/bp_alphaeuw_monsterlist.bp_alphaeuw_monsterlist'");
	FName tabID;
}

void UEditorView_InGameTaskEditor::OnSelectedActor()
{
	SetDetail();
}

void UEditorView_InGameTaskEditor::OnSelectedMonster()
{
	SetDetail();
}

void UEditorView_InGameTaskEditor::OnClicked_Close()
{
	if (false == OnClosedEvent.IsBound())
		return;

	OnClosedEvent.Execute(this);
}

void UEditorView_InGameTaskEditor::OnClicked_Save()
{
	if (false == IsValid(EntryData))
		return;

	FString strTitle = TextBox_Title->GetText().ToString();
	FString strDesc = TextBox_Desc->GetText().ToString();
	int32 step = FCString::Atoi(*TextBox_Step->GetText().ToString()); // Fix: Atof to Atoi
	
	EInGameTaskType type = EInGameTaskType::NONE;
	const UEnum* ptrEnum = StaticEnum<EInGameTaskType>();
	if (ptrEnum)
	{
		FName typeName = FName(*ComboBox_Type->GetSelectedOption());
		type = StaticCast<EInGameTaskType>(ptrEnum->GetValueByName(typeName));
	}

	EntryData->Title = strTitle;
	EntryData->Desc = strDesc;
	EntryData->Step = step;
	EntryData->Type = type;
	//EntryData->TaskDetail
	// Save task-specific details back to the instanced struct
	if (type == EInGameTaskType::SPAWN_OBJECT || type == EInGameTaskType::SPAWN_MONSTER)
	{
		//FMissionTaskData_Spawn* Data = EntryData->TaskDetail.GetMutablePtr<FMissionTaskData_Spawn>();
		//if (Data)
		//{
		//	Data->ActorID = FCString::Atoi(*Txt_SpawnActorID->GetText().ToString());
		//	Data->MonsterID = FCString::Atoi(*Txt_MonsterID->GetText().ToString());
		//}
	}
	EntryData->RefreshUI();
}

void UEditorView_InGameTaskEditor::OnChanged_TaskType(FString inSelectedItem, ESelectInfo::Type inSelectType)
{
	if (false == IsValid(EntryData))
		return;

	EInGameTaskType type = EInGameTaskType::NONE;
	const UEnum* ptrEnum = StaticEnum<EInGameTaskType>();
	if (ptrEnum)
	{
		FName typeName = FName(*ComboBox_Type->GetSelectedOption());
		EntryData->Type = StaticCast<EInGameTaskType>(ptrEnum->GetValueByName(typeName));
		SetDetail();
	}
}
