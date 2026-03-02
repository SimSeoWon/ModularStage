// Fill out your copyright notice in the Description page of Project Settings.


#include "Alpha_HexagonTouchWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/Button.h"

void UAlpha_HexagonTouchWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (IsValid(Btn_Test)) 
	{
		Btn_Test->OnClicked.AddDynamic(this, &UAlpha_HexagonTouchWidget::OnClickedTestBtn);
	}
	
}

void UAlpha_HexagonTouchWidget::OnClickedTestBtn() 
{
	if (false == IsValid(Btn_Test))
		return;
}

FReply UAlpha_HexagonTouchWidget::NativeOnTouchMoved(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	return Super::NativeOnTouchMoved(InGeometry, InGestureEvent);
}

FReply UAlpha_HexagonTouchWidget::NativeOnTouchStarted(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	OnTouchStart.ExecuteIfBound(InGeometry.AbsoluteToLocal(InGestureEvent.GetScreenSpacePosition()));
	return Super::NativeOnTouchStarted(InGeometry, InGestureEvent);
}

FReply UAlpha_HexagonTouchWidget::NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent)
{
	OnTouchEnd.ExecuteIfBound(InGeometry.AbsoluteToLocal(InGestureEvent.GetScreenSpacePosition()));
	return Super::NativeOnTouchEnded(InGeometry, InGestureEvent);
}

FReply UAlpha_HexagonTouchWidget::NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return Super::NativeOnMouseWheel(InGeometry, InMouseEvent);
}

void UAlpha_HexagonTouchWidget::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	const FGeometry& geometry = GetCachedGeometry();
	OnTouchEnd.ExecuteIfBound(geometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition()));
	Super::NativeOnMouseLeave(InMouseEvent);
}


