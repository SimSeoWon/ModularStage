// Fill out your copyright notice in the Description page of Project Settings.
#include "Alpha_HexagonTouchButton.h"
#include <Components/ButtonSlot.h>

FReply SAlpha_HexagonTouchButton::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	//TouchStartPosition = MouseEvent.GetScreenSpacePosition();
	TouchStartPosition = MyGeometry.AbsoluteToLocal(MouseEvent.GetScreenSpacePosition());
	SButton::OnMouseButtonDown(MyGeometry, MouseEvent);
	FReply Reply = FReply::Unhandled();
	return Reply;
}

FReply SAlpha_HexagonTouchButton::OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	TouchEndPosition = MyGeometry.AbsoluteToLocal(MouseEvent.GetScreenSpacePosition());
	return SButton::OnMouseButtonUp(MyGeometry, MouseEvent);
}

FReply SAlpha_HexagonTouchButton::OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	FReply Reply = FReply::Unhandled();
	if (IsPressed()) 
	{
		TouchMovePosition = MyGeometry.AbsoluteToLocal(MouseEvent.GetScreenSpacePosition());
		OnTouchMovedEvent.Execute();
	}

	return Reply;
	// -----------------------------------------------------------------------------------------------------------------------------------------------
}

void SAlpha_HexagonTouchButton::SetOnTouchMovedListener(const FSimpleDelegate& inListener) 
{
	OnTouchMovedEvent = inListener;
}

void SAlpha_HexagonTouchButton::OnMouseLeave(const FPointerEvent& MouseEvent)
{
	SButton::OnMouseLeave(MouseEvent);
	const bool bWasHovered = IsHovered();

	// Call parent implementation
	SWidget::OnMouseLeave(MouseEvent);
	if (bWasHovered)
	{
	}

	Invalidate(EInvalidateWidget::Layout);
}

void SAlpha_HexagonTouchButton::Press()
{
	SButton::Press();
}

void SAlpha_HexagonTouchButton::Release()
{
	SButton::Release();
}

void SAlpha_HexagonTouchButton::ReleaseButtonAndCapture(FReply& InReply)
{
	if (InReply.GetMouseCaptor().IsValid() == false && HasMouseCapture())
	{
		InReply.ReleaseMouseCapture();
	}
	Release();
}

TSharedRef<SWidget> UAlpha_HexagonTouchButton::RebuildWidget()
{
	TSharedPtr<SAlpha_HexagonTouchButton> touchButton = SNew(SAlpha_HexagonTouchButton)
		.OnClicked(BIND_UOBJECT_DELEGATE(FOnClicked, SlateHandleClicked))
		.OnPressed(BIND_UOBJECT_DELEGATE(FSimpleDelegate, SlateHandlePressed))
		.OnReleased(BIND_UOBJECT_DELEGATE(FSimpleDelegate, SlateHandleReleased))
		.OnHovered_UObject(this, &ThisClass::SlateHandleHovered)
		.OnUnhovered_UObject(this, &ThisClass::SlateHandleUnhovered)
		.ButtonStyle(&WidgetStyle)
		.ClickMethod(ClickMethod)
		.TouchMethod(TouchMethod)
		.PressMethod(PressMethod)
		.IsFocusable(IsFocusable)
		;

	touchButton->SetOnTouchMovedListener(BIND_UOBJECT_DELEGATE(FSimpleDelegate, SlateHandleTouchMoved));
	MyButton = touchButton;

	if (GetChildrenCount() > 0)
	{
		Cast<UButtonSlot>(GetContentSlot())->BuildSlot(MyButton.ToSharedRef());
	}

	return MyButton.ToSharedRef();
}

void UAlpha_HexagonTouchButton::SlateHandleTouchMoved() 
{
	OnTouchMove.Broadcast();
}

FVector2D UAlpha_HexagonTouchButton::GetTouchStartPosition()
{
	SAlpha_HexagonTouchButton* myTouchButton = static_cast<SAlpha_HexagonTouchButton*>(MyButton.Get());
	if (myTouchButton == nullptr)
		return FVector2D::ZeroVector;

	return myTouchButton->GetTouchStartPosition();
}

FVector2D UAlpha_HexagonTouchButton::GetTouchMovePosition()
{
	SAlpha_HexagonTouchButton* myTouchButton = static_cast<SAlpha_HexagonTouchButton*>(MyButton.Get());
	if (myTouchButton == nullptr)
		return FVector2D::ZeroVector;

	return myTouchButton->GetTouchMovePosition();
}

FVector2D UAlpha_HexagonTouchButton::GetTouchEndPosition()
{
	SAlpha_HexagonTouchButton* myTouchButton = static_cast<SAlpha_HexagonTouchButton*>(MyButton.Get());
	if (myTouchButton == nullptr)
		return FVector2D::ZeroVector;

	return myTouchButton->GetTouchEndPosition();
}