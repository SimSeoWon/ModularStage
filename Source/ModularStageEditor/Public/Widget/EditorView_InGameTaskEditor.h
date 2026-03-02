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
class UEditorView_MissionTaskEditor;
class UEditorView_SpawnMonsterTaskEditor;
class UEditorView_SpawnObjectTask;


enum class EInGameTaskType : uint8;
/**
 * 
 */
UCLASS()
class UEditorView_InGameTaskEditor : public UEditorUtilityWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

public :
	void SetData(UEntryData_InGameTaskList* inEntryData);

	DECLARE_DELEGATE_OneParam(OnEditorEventDelegate, UEditorView_InGameTaskEditor*)
	OnEditorEventDelegate& GetOnClosedEvent()
	{
		return OnClosedEvent;
	}

	UFUNCTION()
	void OnClicked_Save();

	UFUNCTION()
	void OnClicked_Close();

	UFUNCTION()
	void OnClicked_MonsterList();

	UFUNCTION()
	void OnClicked_Btn_ActorList();

	UFUNCTION()
	void OnChanged_TaskType(FString inSelectedItem, ESelectInfo::Type inSelectType);

	UFUNCTION()
	void OnSelected_HexagonTile(const FVector2D inPos = FVector2D::Zero());

	UFUNCTION()
	void OnSelectedMonster();

	UFUNCTION()
	void OnSelectedActor();

#pragma region Setting Detail widget 
protected:
	void OnRegisterFunctionMap();

	UFUNCTION()
	void SetDetail();

	UFUNCTION()
	void SetDetail_NONE();

	UFUNCTION()
	void SetDetail_SPAWN_MONSTER();

	UFUNCTION()
	void SetDetail_SPAWN_OBJECT();

	UFUNCTION()
	void SetDetail_MISSION();

protected:
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* Switcher_Detail = nullptr;

	UPROPERTY(meta = (BindWidget))
	UPanelWidget* Panel_Empty = nullptr;

	UPROPERTY(meta = (BindWidget))
	UPanelWidget* Panel_SpawnMonster = nullptr;

	UPROPERTY(meta = (BindWidget))
	UEditorView_SpawnMonsterTaskEditor* SpawnMonster = nullptr;

	UPROPERTY(meta = (BindWidget))
	UPanelWidget* Panel_SpawnObject = nullptr;

	UPROPERTY(meta = (BindWidget))
	UEditorView_SpawnObjectTask* SpawnObject = nullptr;

	UPROPERTY(meta = (BindWidget))
	UPanelWidget* Panel_Mission = nullptr;

	UPROPERTY(meta = (BindWidget))
	UEditorView_MissionTaskEditor* MissionTaskEditor = nullptr;

	TMap<EInGameTaskType, UFunction*> FunctionMap;
#pragma endregion

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
	UButton* Btn_Close = nullptr;

	UPROPERTY(meta = (BindWidget))
	UButton* Btn_Save = nullptr;


	
	UPROPERTY()
	UEntryData_InGameTaskList* EntryData = nullptr;

	UPROPERTY()
	UEUW_InGameTaskEditor* Owner = nullptr;

	


	OnEditorEventDelegate OnClosedEvent;
};
