// Copyright Epic Games, Inc. All Rights Reserved.

#include "Widget/TaskListEditorWidget.h"
#include "Components/ListView.h"
#include "Components/Button.h"
#include "ModularStage/Mission/MissionPrefab.h"
#include "PropertyEditorModule.h"
#include "Widgets/Layout/SSplitter.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SBorder.h"
#include "Styling/AppStyle.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "ScopedTransaction.h"

#include "EditorView_InGameTaskEditor.h"
#include "UIListItem_InGameTask.h"

void UTaskListEditorWidget::NativeConstruct()
{
	Super::NativeConstruct();
	CurrEntryData = nullptr;
	Btn_Add->OnClicked.AddDynamic(this, &UTaskListEditorWidget::OnClicked_AddTask);
	Btn_Save->OnClicked.AddDynamic(this, &UTaskListEditorWidget::OnClicked_SaveTask);

	DetailEditor->GetOnClosedEvent().BindUObject(this, &UTaskListEditorWidget::OnClosedDetail);
}

void UTaskListEditorWidget::OnClosedDetail(UEditorView_InGameTaskEditor* inEditorView) 
{
	if (false == IsValid(inEditorView))
		return;

	if (IsValid(CurrEntryData))
	{
		CurrEntryData->IsSelect = false;
		CurrEntryData->RefreshUI();
		CurrEntryData = nullptr;
	}

	RefreshUI();
}

void UTaskListEditorWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UTaskListEditorWidget::BeginDestroy()
{
	Super::BeginDestroy();
}

void UTaskListEditorWidget::SetTargetActor(AMissionPrefab* InActor)
{
	TargetActor = InActor;

	if (false == IsValid(TargetActor))
		return;

	for (const FMissionTaskInfo& iter : TargetActor->TaskList)
	{
		UEntryData_InGameTaskList* entryData = NewObject<UEntryData_InGameTaskList>();
		if (false == IsValid(entryData))
			continue;


		entryData->Step = iter.Step;
		entryData->Title = iter.Title;
		entryData->Desc = iter.Desc;
		entryData->Type = iter.Type;
		entryData->SerializeID = iter.SerializeID;
		entryData->ParentID = iter.ParentID;
		entryData->TaskDetail = iter.TaskDetail;

		entryData->OnClicked_Edit.Unbind();
		entryData->OnClicked_Edit.BindUObject(this, &UTaskListEditorWidget::OnClicked_TaskEdit);

		entryData->OnClicked_Up.Unbind();
		entryData->OnClicked_Up.BindUObject(this, &UTaskListEditorWidget::OnClicked_TaskUp);

		entryData->OnClicked_Down.Unbind();
		entryData->OnClicked_Down.BindUObject(this, &UTaskListEditorWidget::OnClicked_TaskDown);

		entryData->OnClicked_Delete.Unbind();
		entryData->OnClicked_Delete.BindUObject(this, &UTaskListEditorWidget::OnClicked_TaskDelete);
		EntryDataList.Add(entryData);
	}

	List_TaskInfo->SetListItems(EntryDataList);
}

void UTaskListEditorWidget::RefreshUI()
{
	if (IsValid(CurrEntryData))
	{
		DetailEditor->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		DetailEditor->SetData(CurrEntryData);
	}
	else
	{
		DetailEditor->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UTaskListEditorWidget::OnClicked_SaveTask()
{
	TargetActor->SetTaskList(EntryDataList);

}

void UTaskListEditorWidget::OnClicked_AddTask()
{
	UEntryData_InGameTaskList* entryData = NewObject<UEntryData_InGameTaskList>();
	if (false == IsValid(entryData))
		return;

	entryData->Step = EntryDataList.Num();
	entryData->Title = TEXT("Task Title");
	entryData->Desc = TEXT("Task Desc");
	entryData->Type = EInGameTaskType::NONE;
	entryData->SerializeID = FGuid::NewGuid();
	
	// TaskDetail will be initialized when its type is set in the editor view.

	entryData->OnClicked_Edit.BindUObject(this, &UTaskListEditorWidget::OnClicked_TaskEdit);
	entryData->OnClicked_Delete.BindUObject(this, &UTaskListEditorWidget::OnClicked_TaskDelete);
	entryData->OnClicked_Up.BindUObject(this, &UTaskListEditorWidget::OnClicked_TaskUp);
	entryData->OnClicked_Down.BindUObject(this, &UTaskListEditorWidget::OnClicked_TaskDown);
	EntryDataList.Add(entryData);

	List_TaskInfo->SetListItems(EntryDataList);

	OnClicked_TaskEdit(entryData);
}

void UTaskListEditorWidget::OnClicked_TaskDown(UEntryData_InGameTaskList* inEntryData)
{
	if (false == IsValid(inEntryData))
		return;
}

void UTaskListEditorWidget::OnClicked_TaskUp(UEntryData_InGameTaskList* inEntryData)
{
	if (false == IsValid(inEntryData))
		return;
}

void UTaskListEditorWidget::OnClicked_TaskDelete(UEntryData_InGameTaskList* inEntryData)
{
	if (false == IsValid(inEntryData))
		return;

	if (CurrEntryData == inEntryData)
	{
		CurrEntryData = nullptr;
	}

	EntryDataList.Remove(inEntryData);
	List_TaskInfo->SetListItems(EntryDataList);
	RefreshUI();
}

void UTaskListEditorWidget::OnClicked_TaskEdit(UEntryData_InGameTaskList* inEntryData)
{
	if (false == IsValid(inEntryData))
		return;

	TObjectPtr<UEntryData_InGameTaskList> prevEntryData = CurrEntryData;
	CurrEntryData = inEntryData;
	CurrEntryData->IsSelect = true;
	CurrEntryData->RefreshUI();

	if (IsValid(prevEntryData) && prevEntryData != CurrEntryData)
	{
		prevEntryData->IsSelect = false;
		prevEntryData->RefreshUI();
	}

	RefreshUI();
}
