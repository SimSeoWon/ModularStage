// Fill out your copyright notice in the Description page of Project Settings.


#include "EUW_InGameTaskEditor.h"
#include "EditorView_InGameTaskEditor.h"
#include "UIListItem_InGameTask.h"

void UEUW_InGameTaskEditor::Run() 
{
	EditorView->Run(this);
}

void UEUW_InGameTaskEditor::SetData(UEntryData_InGameTaskList* inEntryData) 
{
	if (false == IsValid(inEntryData))
		return;

	EditorView->SetData(inEntryData);
}

TArray<UAlphaEntryData_MonsterSquad*> UEUW_InGameTaskEditor::GetSpawnMonsterList()
{
	TArray<UAlphaEntryData_MonsterSquad*> result;
	if (Function_SpawnMonsterList)
	{
		Function_SpawnMonsterList(result);
	}

	return result;
}