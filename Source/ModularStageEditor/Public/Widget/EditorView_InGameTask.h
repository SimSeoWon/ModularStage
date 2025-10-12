// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "EditorView_InGameTask.generated.h"

class UDataTable;
class UListView;
class UButton;
class UEditableTextBox;
class UEntryData_InGameTaskList;
/**
 *
 */
UCLASS()
class UEditorView_InGameTask : public UEditorUtilityWidget
{
	GENERATED_BODY()

public:
	void Run();
	void CreateTableFile(FString inFileName);
	void SetTableData(UDataTable* inDataTable);

	UFUNCTION()
	void OnClicked_LoadFile();

	UFUNCTION()
	void OnClicked_SaveFile();

	UFUNCTION()
	void OnClicked_AddTask();

	UFUNCTION()
	void OnClicked_TaskEdit(UEntryData_InGameTaskList* inEntryData);

protected:
	void SetMonsterList();

protected:
	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* Edit_TablePath = nullptr;

	UPROPERTY(meta = (BindWidget))
	UButton* Btn_SaveFile = nullptr;

	UPROPERTY(meta = (BindWidget))
	UButton* Btn_LoadFile = nullptr;

	UPROPERTY(meta = (BindWidget))
	UButton* Btn_AddTask = nullptr;

	UPROPERTY(meta = (BindWidget))
	UListView* List_TaskInfo = nullptr;

	FString FilePath;
	FString FileName;

	UPROPERTY()
	TArray<UEntryData_InGameTaskList*> EntryDataList;
	
};
