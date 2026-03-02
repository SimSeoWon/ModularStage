// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "EditorView_MissionTaskEditor.generated.h"

class UWidgetSwitcher;
class UPanelWidget;
class UEditableTextBox;
class UTextBlock;
class UButton;
class UComboBoxString;
class UEntryData_InGameTaskList;
class UEUW_InGameTaskEditor;

enum class EInGameTaskType : uint8;
enum class EMissionObjectiveType : uint8;
/**
 * 
 */
UCLASS()
class UEditorView_MissionTaskEditor : public UEditorUtilityWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION()
	void OnChanged_TaskType(FString inSelectedItem, ESelectInfo::Type inSelectType);

public :
	void SetData(UEntryData_InGameTaskList* inEntryData);

protected:

	UPROPERTY(meta = (BindWidget))
	UComboBoxString* ComboBox_Type = nullptr;

	UPROPERTY()
	UEntryData_InGameTaskList* EntryData = nullptr;


#pragma region Setting Detail widget
protected:
	void OnRegisterFunctionMap();

	UFUNCTION()
	void SetDetail();

	UFUNCTION()
	void SetDetail_None();

	UFUNCTION()
	void SetDetail_Destination();

	UFUNCTION()
	void SetDetail_Annihilation();

protected:
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* Switcher_Detail = nullptr;

	UPROPERTY(meta = (BindWidget))
	UPanelWidget* Panel_Empty = nullptr;

	UPROPERTY(meta = (BindWidget))
	UPanelWidget* Panel_Destination = nullptr;

	UPROPERTY(meta = (BindWidget))
	UPanelWidget* Panel_Annihilation = nullptr;

	TMap<EMissionObjectiveType, UFunction*> FunctionMap;
#pragma endregion

};
