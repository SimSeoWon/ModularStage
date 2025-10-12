// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "EUW_InGameTaskEditor.generated.h"

class UAlphaEntryData_MonsterSquad;
class UEditorView_InGameTaskEditor;
class UEntryData_InGameTaskList;

/**
 * 
 */
UCLASS()
class UEUW_InGameTaskEditor : public UEditorUtilityWidget
{
	GENERATED_BODY()

public:
	void Run();
	void SetData(UEntryData_InGameTaskList* inEntryData);
	
	void SetFunction_SpawnMonsterList(TFunction<void(TArray<UAlphaEntryData_MonsterSquad*>&)> inFunction)
	{
		Function_SpawnMonsterList = inFunction;
	}

	TArray<UAlphaEntryData_MonsterSquad*> GetSpawnMonsterList();

protected:
	UPROPERTY(meta = (BindWidget))
	UEditorView_InGameTaskEditor* EditorView = nullptr;

	TFunction<void(TArray<UAlphaEntryData_MonsterSquad*>&)> Function_SpawnMonsterList = nullptr;        //스폰된 몬스터 목록을 가져오자.
	
};
