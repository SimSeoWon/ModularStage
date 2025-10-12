// Fill out your copyright notice in the Description page of Project Settings.


#include "EditorView_InGameTaskEditor.h"
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

void UEditorView_InGameTaskEditor::Run(UEUW_InGameTaskEditor* inOwner)
{
	Owner = inOwner;

	OnRegisterFunctionMap();

	if (Btn_Save->OnClicked.IsBound())
		Btn_Save->OnClicked.Clear();
	Btn_Save->OnClicked.AddDynamic(this, &UEditorView_InGameTaskEditor::OnClicked_Save);

	//if (Btn_MonsterList->OnClicked.IsBound())
	//	Btn_MonsterList->OnClicked.Clear();
	//Btn_MonsterList->OnClicked.AddDynamic(this, &UEditorView_InGameTaskEditor::OnClicked_MonsterList);

	//if (Btn_ActorList->OnClicked.IsBound())
	//	Btn_ActorList->OnClicked.Clear();

	//Btn_ActorList->OnClicked.AddDynamic(this, &UEditorView_InGameTaskEditor::OnClicked_Btn_ActorList);

	if (ComboBox_Type->OnSelectionChanged.IsBound())
		ComboBox_Type->OnSelectionChanged.Clear();
	ComboBox_Type->OnSelectionChanged.AddDynamic(this, &UEditorView_InGameTaskEditor::OnChanged_TaskType);

	const UEnum* ptrEnum = FindObject<UEnum>(ANY_PACKAGE, TEXT("EInGameTaskType"), true);
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

	LoadFile();

}

void UEditorView_InGameTaskEditor::LoadFile()
{
	FString strFileName = TEXT("");
	FString strFilePath = TEXT("");

	UWorld* world = GetWorld();
	if (IsValid(world))
	{
		FString strWorldName;
		world->GetName(strWorldName);
		if (false == strWorldName.IsEmpty())
		{
			strFileName = FString::Printf(TEXT("res_%s"), *strWorldName);
			strFilePath = FString::Printf(TEXT("DataTable'/Game/GameData/resourcetable/%s.%s'"), *strFileName, *strFileName);
		}
	}

	float tileSize = 0.0f;
	int32 column = 0;
	int32 count = 0;

	UDataTable* dtHexagonTileMap = LoadObject<UDataTable>(this, *strFilePath);
	if (nullptr == dtHexagonTileMap)
		return;

	FMissionTaskTable* rowData = dtHexagonTileMap->FindRow<FMissionTaskTable>(FName(TEXT("TEST")), TEXT(""));
	if (nullptr == rowData)
		return;

}

void UEditorView_InGameTaskEditor::OnRegisterFunctionMap()
{
	FunctionMap.Reset();
	const UEnum* ptrEnum = FindObject<UEnum>(ANY_PACKAGE, TEXT("EInGameTaskType"), true);
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

#pragma region Setting Detail widget 
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

void UEditorView_InGameTaskEditor::SetDetail_MARCH_ON()
{
	/*Switcher_Detail->SetActiveWidget(Panel_MarchOn);
	if (false == IsValid(EntryData))
		return;

	EntryData->IntegerParamList.Reserve(1);
	if (false == EntryData->IntegerParamList.IsValidIndex(0))
	{
		EntryData->IntegerParamList.Emplace(0);
	}

	int32 id = EntryData->IntegerParamList[0];*/
	
}

void UEditorView_InGameTaskEditor::SetDetail_SPAWN()
{
	Switcher_Detail->SetActiveWidget(Panel_SPAWN);
	if (false == IsValid(EntryData))
		return;

	/*EntryData->IntegerParamList.Reserve(2);
	if (false == EntryData->IntegerParamList.IsValidIndex(0))
		EntryData->IntegerParamList.Add(0);
	int32 actorId = EntryData->IntegerParamList[0];
	Txt_SpawnActorID->SetText(FText::AsNumber(actorId));

	if (false == EntryData->IntegerParamList.IsValidIndex(1))
		EntryData->IntegerParamList.Add(0);
	int32 monsterId = EntryData->IntegerParamList[1];
	Txt_MonsterID->SetText(FText::AsNumber(monsterId));*/
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
	const UEnum* ptrEnum = FindObject<UEnum>(ANY_PACKAGE, TEXT("EInGameTaskType"), true);
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

	const FString DeleteInactiveEmitterUIPath = TEXT("EditorUtilityWidgetBlueprint'/Game/nevercook/editor/bp_alphaeuw_actorlist.bp_alphaeuw_actorlist'");
	FName tabID;
}

void UEditorView_InGameTaskEditor::OnClicked_MonsterList()
{
	if (false == IsValid(EntryData))
		return;

	const FString DeleteInactiveEmitterUIPath = TEXT("EditorUtilityWidgetBlueprint'/Game/nevercook/editor/bp_alphaeuw_monsterlist.bp_alphaeuw_monsterlist'");
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

void UEditorView_InGameTaskEditor::OnClicked_Save()
{
	if (false == IsValid(EntryData))
		return;

	FString strTitle = TextBox_Title->GetText().ToString();
	FString strDesc = TextBox_Desc->GetText().ToString();
	int32 step = FCString::Atof(*TextBox_Step->GetText().ToString());
	//int32 index = FCString::Atoi(*Txt_TileIndex->GetText().ToString());
	EInGameTaskType type = EInGameTaskType::NONE;
	const UEnum* ptrEnum = FindObject<UEnum>(ANY_PACKAGE, TEXT("EInGameTaskType"), true);
	if (ptrEnum)
	{
		FName typeName = FName(*ComboBox_Type->GetSelectedOption());
		type = StaticCast<EInGameTaskType>(ptrEnum->GetValueByName(typeName));
	}

	EntryData->Title = strTitle;
	EntryData->Desc = strDesc;
	EntryData->Step = step;
	EntryData->Type = type;
	//EntryData->Index = index;
	EntryData->RefreshUI(); //에디터에 UI를 갱신한다.
}

void UEditorView_InGameTaskEditor::OnChanged_TaskType(FString inSelectedItem, ESelectInfo::Type inSelectType)
{
	if (false == IsValid(EntryData))
		return;

	EInGameTaskType type = EInGameTaskType::NONE;
	const UEnum* ptrEnum = FindObject<UEnum>(ANY_PACKAGE, TEXT("EInGameTaskType"), true);
	if (ptrEnum)
	{
		FName typeName = FName(*ComboBox_Type->GetSelectedOption());
		EntryData->Type = StaticCast<EInGameTaskType>(ptrEnum->GetValueByName(typeName));
		SetDetail();
	}
}

void UEditorView_InGameTaskEditor::OnChanged_TileIndex(FString inSelectedItem, ESelectInfo::Type inSelectType)
{

}