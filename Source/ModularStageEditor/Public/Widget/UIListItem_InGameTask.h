// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularStage/UI/Base/UIWidgetBase.h"
#include <Blueprint/IUserObjectListEntry.h>
#include "InstancedStruct.h"
#include "ModularStage/Table/MissionTaskData.h"
#include "UIListItem_InGameTask.generated.h"

class UTextBlock;
class UButton;
class UImage;
enum class EInGameTaskType : uint8;
class UEntryData_InGameTaskList;
/**
 *
 */


UCLASS()
class UUIListItem_InGameTask : public UUIWidgetBase, public IUserObjectListEntry
{
	GENERATED_BODY()
protected:

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeOnListItemObjectSet(UObject* ListItemObject) override;
	void SetData(UEntryData_InGameTaskList* inEntryData);

public:
	UFUNCTION()
	void OnClicked_Up();

	UFUNCTION()
	void OnClicked_Down();

	UFUNCTION()
	void OnClicked_Delete();

	UFUNCTION()
	void OnClicked_Edit();

	void OnRefleshUI();

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Txt_Step = nullptr;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Txt_Title = nullptr;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Txt_Type = nullptr;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Txt_Desc = nullptr;

	UPROPERTY(meta = (BindWidget))
	UButton* Btn_Up = nullptr;

	UPROPERTY(meta = (BindWidget))
	UButton* Btn_Down = nullptr;

	UPROPERTY(meta = (BindWidget))
	UButton* Btn_Delete = nullptr;

	UPROPERTY(meta = (BindWidget))
	UButton* Btn_Edit = nullptr;

	UPROPERTY(meta = (BindWidget))
	UImage* Img_Select = nullptr;

	UPROPERTY()
	UEntryData_InGameTaskList* EntryData = nullptr;
};

UCLASS()
class UEntryData_InGameTaskList : public UObject
{
	GENERATED_BODY()
public:
	void RefreshUI();
	void OnReqUp();
	void OnReqDown();
	void OnReqDelete();
	void OnShowEditor();

	FString GetTypeName();

public:

	DECLARE_DELEGATE_OneParam(FOnClickedDelegate, UEntryData_InGameTaskList*);

	FOnClickedDelegate OnClicked_Up = nullptr;
	FOnClickedDelegate OnClicked_Down = nullptr;
	FOnClickedDelegate OnClicked_Delete = nullptr;
	FOnClickedDelegate OnClicked_Edit = nullptr;

	int32 Step = 0;
	FString Title;
	FString Desc;
	int32 Index = -1;
	FVector2D Coordinates;
	FGuid ParentID;
	FGuid SerializeID;
	EInGameTaskType Type;
	bool IsSelect = false;

	UPROPERTY(EditAnywhere, meta = (BaseStruct = "/Script/ModularStage.MissionTaskData_Base"))
	FInstancedStruct TaskDetail;

	TArray<UEntryData_InGameTaskList*>  SubTaskList;
	UPROPERTY()
	UUIListItem_InGameTask* Widget = nullptr;
};