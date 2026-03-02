// Fill out your copyright notice in the Description page of Project Settings.


#include "UIListItem_InGameTask.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Support/EditorSupport.h"
#include "EUW_InGameTaskEditor.h"


FString UEntryData_InGameTaskList::GetTypeName()
{
	const UEnum* ptrEnum = StaticEnum<EInGameTaskType>();
	if (ptrEnum)
	{
		return ptrEnum->GetNameStringByValue((int64)Type);
	}
	
	return TEXT("");
}

void UEntryData_InGameTaskList::RefreshUI()
{
	if (false == IsValid(Widget))
		return;

	Widget->OnRefleshUI();
}

void UEntryData_InGameTaskList::OnReqUp()
{
	if (false == OnClicked_Up.IsBound())
		return;

	OnClicked_Up.Execute(this);
}

void UEntryData_InGameTaskList::OnReqDown()
{
	if (false == OnClicked_Down.IsBound())
		return;

	OnClicked_Down.Execute(this);
}

void UEntryData_InGameTaskList::OnReqDelete() 
{
	if (false == OnClicked_Delete.IsBound())
		return;

	OnClicked_Delete.Execute(this);
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
	Txt_Type->SetText(FText::FromString(inEntryData->GetTypeName()));
	Txt_Desc->SetText(FText::FromString(inEntryData->Desc));
	Img_Select->SetVisibility(inEntryData->IsSelect ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);

}

void UUIListItem_InGameTask::OnClicked_Up()
{
	if (false == IsValid(EntryData))
		return;

	EntryData->OnReqUp();

}

void UUIListItem_InGameTask::OnClicked_Down()
{
	if (false == IsValid(EntryData))
		return;

	EntryData->OnReqDown();

}

void UUIListItem_InGameTask::OnClicked_Delete()
{
	if (false == IsValid(EntryData))
		return;

	EntryData->OnReqDelete();
}

void UUIListItem_InGameTask::OnClicked_Edit()
{
	if (false == IsValid(EntryData))
		return;

	EntryData->OnShowEditor();
}
