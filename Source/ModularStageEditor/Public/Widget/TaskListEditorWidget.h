// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Widget/EditorWidgetBase.h"
#include "Widgets/Views/SListView.h"
#include "ModularStage/Table/MissionTaskTable.h"
#include "TaskListEditorWidget.generated.h"

class UListView;
class UButton;
class AMissionPrefab;
class UEntryData_InGameTaskList;
class UEditorView_InGameTaskEditor;

/**
 * A dedicated editor widget for adding, removing, and editing FMissionTaskInfo entries
 * on a target object's TaskList property.
 */
UCLASS()
class MODULARSTAGEEDITOR_API UTaskListEditorWidget : public UEditorWidgetBase
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void BeginDestroy() override;


public:
	static FName GetMenuName() { return FName(TEXT("Task List Editor")); }
	static FText GetMenuDisplayName() { return FText::FromString(TEXT("Task List Editor")); }
	static FText GetMenuTooltip() { return FText::FromString(TEXT("Tool for creating and editing AMissionPrefab Task List")); }

	// Ensure this BP is created in the editor later
	static FString GetWidgetBlueprintPath() { return TEXT("/Game/nevercook/editor/menu/BP_TaskListEditorWidget.BP_TaskListEditorWidget"); }

public:
	void RefreshUI();

	/** Sets the target actor whose TaskList we are editing. */
	void SetTargetActor(AMissionPrefab* InActor);

	UFUNCTION()
	void OnClicked_AddTask();

	UFUNCTION()
	void OnClicked_SaveTask();

	UFUNCTION()
	void OnClicked_TaskEdit(UEntryData_InGameTaskList* inEntryData);

	UFUNCTION()
	void OnClicked_TaskDown(UEntryData_InGameTaskList* inEntryData);

	UFUNCTION()
	void OnClicked_TaskUp(UEntryData_InGameTaskList* inEntryData);

	UFUNCTION()
	void OnClicked_TaskDelete(UEntryData_InGameTaskList* inEntryData);

	UFUNCTION()
	void OnClosedDetail(UEditorView_InGameTaskEditor* inEditorView);

private:
	UPROPERTY(meta = (BindWidget))
	UButton* Btn_Add = nullptr;

	UPROPERTY(meta = (BindWidget))
	UButton* Btn_Save = nullptr;

	UPROPERTY(meta = (BindWidget))
	UEditorView_InGameTaskEditor* DetailEditor = nullptr;

	UPROPERTY(meta = (BindWidget))
	UListView* List_TaskInfo = nullptr;

	UPROPERTY()
	TObjectPtr<AMissionPrefab> TargetActor;

	UPROPERTY()
	TArray<UEntryData_InGameTaskList*> EntryDataList;

	UPROPERTY()
	TObjectPtr<UEntryData_InGameTaskList> CurrEntryData = nullptr;
};
