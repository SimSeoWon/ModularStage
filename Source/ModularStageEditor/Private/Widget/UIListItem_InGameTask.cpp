// Fill out your copyright notice in the Description page of Project Settings.


#include "UIListItem_InGameTask.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

#include "Support/EditorSupport.h"
#include "EUW_InGameTaskEditor.h"

void UEntryData_InGameTaskList::RefreshUI() 
{
	if (false == IsValid(Widget))
		return;

	Widget->OnRefleshUI();
}

void UEntryData_InGameTaskList::OnShowEditor()
{
	if (false == OnClicked_Edit.IsBound())
		return;

	OnClicked_Edit.Execute(this);
}

void UUIListItem_InGameTask::NativeConstruct() 
{
	UUserWidget::NativeConstruct();

	Btn_Up->OnClicked.AddDynamic(this, &UUIListItem_InGameTask::OnClicked_Up);
	Btn_Down->OnClicked.AddDynamic(this, &UUIListItem_InGameTask::OnClicked_Down);
	Btn_Delete->OnClicked.AddDynamic(this, &UUIListItem_InGameTask::OnClicked_Delete);
	Btn_Edit->OnClicked.AddDynamic(this, &UUIListItem_InGameTask::OnClicked_Edit);
}

void UUIListItem_InGameTask::NativeDestruct() 
{
	UUserWidget::NativeDestruct();
}

void UUIListItem_InGameTask::NativeOnListItemObjectSet(UObject* inListItemObject)
{
	if (false == IsValid(inListItemObject))
		return;

	UEntryData_InGameTaskList* data = Cast<UEntryData_InGameTaskList>(inListItemObject);
	if (false == IsValid(data))
		return;

	data->Widget = this;
	SetData(data);
}

void UUIListItem_InGameTask::OnRefleshUI()
{
	if (false == IsValid(EntryData))
		return;

	SetData(EntryData);
}

void UUIListItem_InGameTask::SetData(UEntryData_InGameTaskList* inEntryData)
{
	if (false == IsValid(inEntryData))
		return;

	EntryData = inEntryData;
	

	Txt_Step->SetText(FText::AsNumber(inEntryData->Step));
	Txt_Title->SetText(FText::FromString(inEntryData->Title));
	Txt_Desc->SetText(FText::FromString(inEntryData->Desc));
}

void UUIListItem_InGameTask::OnClicked_Up()
{

}

void UUIListItem_InGameTask::OnClicked_Down()
{

}

void UUIListItem_InGameTask::OnClicked_Delete()
{

}

void UUIListItem_InGameTask::OnClicked_Edit()
{
	if (false == IsValid(EntryData))
		return;

	EntryData->OnShowEditor();
	
}
