// Fill out your copyright notice in the Description page of Project Settings.


#include "EditorView_InGameTask.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "Components/ListView.h"
#include "Misc/DateTime.h"
#include "Engine/DataTable.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "UObject/SavePackage.h"
#include "Editor.h"
#include "EditorModeManager.h"

#include "UIListItem_InGameTask.h"

#include "Support/EditorSupport.h"
#include "EUW_InGameTaskEditor.h"

#include "ModularStage/Table/TableEnum.h"
#include "ModularStage/Table/MissionTaskTable.h"
#include "ModularStage/Table/MissionTaskFunctor.h"


void UEditorView_InGameTask::Run()
{
	FilePath = TEXT("");
	FileName = TEXT("");

	UWorld* world = GetWorld();
	if (IsValid(world))
	{
		FString strWorldName;
		world->GetName(strWorldName);
		if (false == strWorldName.IsEmpty())
		{
			FileName = FString::Printf(TEXT("MissionTask_%s"), *strWorldName);
			FilePath = FString::Printf(TEXT("DataTable'/Game/GameData/resourcetable/%s.%s'"), *FileName, *FileName);
		}
	}

	Edit_TablePath->SetText(FText::FromString(FilePath));

	Btn_SaveFile->OnClicked.Clear();
	Btn_SaveFile->OnClicked.AddDynamic(this, &UEditorView_InGameTask::OnClicked_SaveFile);
	Btn_LoadFile->OnClicked.Clear();
	Btn_LoadFile->OnClicked.AddDynamic(this, &UEditorView_InGameTask::OnClicked_LoadFile);
	Btn_AddTask->OnClicked.Clear();
	Btn_AddTask->OnClicked.AddDynamic(this, &UEditorView_InGameTask::OnClicked_AddTask);

	/*Btn_AddMonster->OnClicked.Clear();
	Btn_AddMonster->OnClicked.AddDynamic(this, &UEditorView_InGameTask::OnClicked_AddMonster);*/

	OnClicked_LoadFile();
	
}

void UEditorView_InGameTask::OnClicked_SaveFile()
{
	//FString FilePath;
	//FString FileName;

	if (IsValid(Edit_TablePath))
		FilePath = Edit_TablePath->GetText().ToString();

	UDataTable* dtHexagonTileMap = LoadObject<UDataTable>(this, *FilePath);
	if (dtHexagonTileMap)
	{
		
	}

	CreateTableFile(FileName);
}

void UEditorView_InGameTask::OnClicked_LoadFile()
{
	//FString FileName = TEXT("");
	//FString FilePath = TEXT("");
	if (IsValid(Edit_TablePath))
		FilePath = Edit_TablePath->GetText().ToString();

	UDataTable* dtInGameTask = LoadObject<UDataTable>(this, *FilePath);
	if (dtInGameTask)
	{
		FMissionTaskTable* rowData = dtInGameTask->FindRow<FMissionTaskTable>(FName(TEXT("TEST")), TEXT(""));
		if (nullptr == rowData)
			return;


		EntryDataList.Reset();
		EntryDataList.Reserve(rowData->TaskList.Num());

		for (int32 i = 0; i < rowData->TaskList.Num(); i++)
		{
			UEntryData_InGameTaskList* entryData = NewObject<UEntryData_InGameTaskList>();
			if (false == IsValid(entryData))
				return;
			
			entryData->Step = rowData->TaskList[i].Step;
			entryData->Title = rowData->TaskList[i].Title;
			entryData->Desc = rowData->TaskList[i].Desc;
			
			entryData->IntegerParamList.Append(rowData->TaskList[i].IntegerParamList);
			entryData->OnClicked_Edit.Unbind();
			entryData->OnClicked_Edit.BindUObject(this, &UEditorView_InGameTask::OnClicked_TaskEdit);
			EntryDataList.Add(entryData);
		}
	}

	List_TaskInfo->SetListItems(EntryDataList);

	SetMonsterList();
}

void UEditorView_InGameTask::SetMonsterList()
{
	for (auto iter : EntryDataList)
	{
		if (false == IsValid(iter))
			continue;

		if(iter->Type != EInGameTaskType::SPAWN_MONSTER)
			continue;

		
	}
}

void UEditorView_InGameTask::OnClicked_AddTask()
{
	UEntryData_InGameTaskList* entryData = NewObject<UEntryData_InGameTaskList>();
	if (false == IsValid(entryData))
		return;
	
	entryData->Step = EntryDataList.Num();
	entryData->Title = TEXT("Task Title");
	entryData->Desc = TEXT("Task Desc");
	entryData->OnClicked_Edit.Unbind();
	entryData->OnClicked_Edit.BindUObject(this, &UEditorView_InGameTask::OnClicked_TaskEdit);
	EntryDataList.Add(entryData);

	List_TaskInfo->SetListItems(EntryDataList);

	OnClicked_TaskEdit(entryData);

}

void UEditorView_InGameTask::OnClicked_TaskEdit(UEntryData_InGameTaskList* inEntryData) 
{
	if (false == IsValid(inEntryData))
		return;

	const FString DeleteInactiveEmitterUIPath = TEXT("EditorUtilityWidgetBlueprint'/Game/nevercook/editor/menu/bp_EUW_InGameTaskdetail.bp_EUW_InGameTaskdetail'");
	FName tabID;
	UEUW_InGameTaskEditor* editorWidget = FEditorSupport::ShowEditorUtilityWidget<UEUW_InGameTaskEditor>(DeleteInactiveEmitterUIPath, tabID);
	if (false == IsValid(editorWidget))
		return;

	editorWidget->Run();
	editorWidget->SetData(inEntryData);

	
}

void UEditorView_InGameTask::CreateTableFile(FString inFileName)
{
	FString packagePath = FString::Printf(TEXT("/Game/GameData/resourcetable/%s"), *inFileName);
	FString assetPath = FPaths::ProjectContentDir() + TEXT("GameData/resourcetable/");
	UPackage* package = CreatePackage(*packagePath);
	if (false == IsValid(package))
		return;

	package->FullyLoad();
	UDataTable* dataTable = NewObject<UDataTable>(package, UDataTable::StaticClass(), *inFileName, EObjectFlags::RF_Public | EObjectFlags::RF_Standalone);
	if (false == IsValid(dataTable))
		return;

	SetTableData(dataTable);
	FAssetRegistryModule::AssetCreated(dataTable);
	package->MarkPackageDirty();

	FString filePath = FString::Printf(TEXT("%s%s%s"), *assetPath, *inFileName, *FPackageName::GetAssetPackageExtension());
	bool bSuccess = UPackage::SavePackage(package, dataTable, EObjectFlags::RF_Public | EObjectFlags::RF_Standalone, *filePath);

	if (bSuccess)
	{
		TArray<UObject*> ObjectsToSync;
		ObjectsToSync.Add(dataTable);
		GEditor->SyncBrowserToObjects(ObjectsToSync);
	}
}

void UEditorView_InGameTask::SetTableData(UDataTable* inDataTable)
{
	if (false == IsValid(inDataTable))
		return;

	inDataTable->RowStruct = FMissionTaskTable::StaticStruct();
	FName rowName = FName(TEXT("TEST"));
	FMissionTaskTable rowData;
	for(int32 i = 0; i < EntryDataList.Num(); i++)
	{
		FMissionTaskInfo taskInfo;
		taskInfo.Step = EntryDataList[i]->Step;
		taskInfo.Title = EntryDataList[i]->Title;
		taskInfo.Desc = EntryDataList[i]->Desc;
		taskInfo.IntegerParamList.Append(EntryDataList[i]->IntegerParamList);
		rowData.TaskList.Add(taskInfo);
	}

	inDataTable->AddRow(rowName, rowData);
}
