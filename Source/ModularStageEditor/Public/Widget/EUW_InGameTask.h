// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget/EditorWidgetBase.h"
#include "EUW_InGameTask.generated.h"

class UEditorView_InGameTask;
/**
 *
 */
UCLASS()
class UEUW_InGameTask : public UEditorWidgetBase
{
	GENERATED_BODY()

public:
	static FName GetMenuName() { return FName(TEXT("Mission Task Editor")); }
	static FText GetMenuDisplayName() { return FText::FromString(TEXT("Mission Task Editor")); }
	static FText GetMenuTooltip() { return FText::FromString(TEXT("Mission Task Editor")); }
	
	static FString GetWidgetBlueprintPath() { return TEXT("/Game/nevercook/editor/BP_EUW_InGameTask.BP_EUW_InGameTask"); }

	void Run();

protected:
	UPROPERTY(meta = (BindWidget))
	UEditorView_InGameTask* Edit_InGameTask = nullptr;

};
