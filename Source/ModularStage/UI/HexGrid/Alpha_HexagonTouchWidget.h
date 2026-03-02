// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UObject/NoExportTypes.h"
#include "Input/Reply.h"
#include "Alpha_HexagonTouchWidget.generated.h"

class UButton;

DECLARE_DELEGATE_OneParam(FHexagonTouchDelegate, const FVector2D)
/**
 * 
 */
UCLASS()
class MODULARSTAGE_API UAlpha_HexagonTouchWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	virtual FReply NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	virtual FReply NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	virtual FReply NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);

	UFUNCTION()
	void OnClickedTestBtn();

public:
	FHexagonTouchDelegate			OnTouchStart;
	FHexagonTouchDelegate			OnTouchEnd;

protected:
	UPROPERTY(meta = (BindWidget))
	UButton* Btn_Test = nullptr;

};
