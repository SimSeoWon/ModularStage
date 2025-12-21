// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "EditorView_InGameTaskEditor.generated.h"

class UWidgetSwitcher;
class UPanelWidget;
class UEditableTextBox;
class UTextBlock;
class UButton;
class UComboBoxString;
class UEntryData_InGameTaskList;
class UEUW_InGameTaskEditor;

enum class EInGameTaskType : uint8;
/**
 * 
 */
UCLASS()
class UEditorView_InGameTaskEditor : public UEditorUtilityWidget
{
	GENERATED_BODY()

public :
	void Run(UEUW_InGameTaskEditor* inOwner);

	void SetData(UEntryData_InGameTaskList* inEntryData);

	UFUNCTION()
	void OnClicked_Save();

	UFUNCTION()
	void OnClicked_MonsterList();

	UFUNCTION()
	void OnClicked_Btn_ActorList();

	UFUNCTION()
	void OnChanged_TaskType(FString inSelectedItem, ESelectInfo::Type inSelectType);

	UFUNCTION()
	void OnChanged_TileIndex(FString inSelectedItem, ESelectInfo::Type inSelectType);

	UFUNCTION()
	void OnSelected_HexagonTile(const FVector2D inPos = FVector2D::Zero());

	UFUNCTION()
	void OnSelectedMonster();

	UFUNCTION()
	void OnSelectedActor();

#pragma region Setting Detail widget 
	UFUNCTION()
	void SetDetail();

	UFUNCTION()
	void SetDetail_NONE();

	UFUNCTION()
	void SetDetail_SPAWN();

	UFUNCTION()
	void SetDetail_MARCH_ON();
#pragma endregion
protected:
	void LoadFile();
	void OnRegisterFunctionMap();
protected:
	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* TextBox_Step = nullptr;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* TextBox_Title = nullptr;

	UPROPERTY(meta = (BindWidget))
	UEditableTextBox* TextBox_Desc = nullptr;

	UPROPERTY(meta = (BindWidget))
	UComboBoxString* ComboBox_Type = nullptr;

	UPROPERTY(meta = (BindWidget))
	UButton* Btn_Save = nullptr;

	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* Switcher_Detail = nullptr;

	UPROPERTY(meta = (BindWidget))
	UPanelWidget* Panel_Empty = nullptr;

	UPROPERTY(meta = (BindWidget))
	UPanelWidget* Panel_SPAWN = nullptr;
	//UPROPERTY(meta = (BindWidget))
	//UTextBlock* Txt_MonsterID = nullptr;
	//UPROPERTY(meta = (BindWidget))
	//UTextBlock* Txt_SpawnActorID = nullptr;
	//UPROPERTY(meta = (BindWidget))
	//UButton* Btn_MonsterList = nullptr;

	//UPROPERTY(meta = (BindWidget))
	//UPanelWidget* Panel_MarchOn = nullptr;
	//UPROPERTY(meta = (BindWidget))
	//UTextBlock* Txt_ActorID = nullptr;
	//UPROPERTY(meta = (BindWidget))
	//UButton* Btn_ActorList = nullptr;

	UPROPERTY()
	UEntryData_InGameTaskList* EntryData = nullptr;

	UPROPERTY()
	UEUW_InGameTaskEditor* Owner = nullptr;

	TMap<EInGameTaskType, UFunction*> FunctionMap;
};
