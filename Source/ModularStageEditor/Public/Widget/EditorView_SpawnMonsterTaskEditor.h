// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "EditorView_SpawnMonsterTaskEditor.generated.h"

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
class UEditorView_SpawnMonsterTaskEditor : public UEditorUtilityWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
};
