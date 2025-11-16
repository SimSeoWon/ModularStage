// Fill out your copyright notice in the Description page of Project Settings.
#include "SquareTouchButton.h"
#include <Components/ButtonSlot.h>

FReply SSquareTouchButton::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	//TouchStartPosition = MouseEvent.GetScreenSpacePosition();
	TouchStartPosition = MyGeometry.AbsoluteToLocal(MouseEvent.GetScreenSpacePosition());
	SButton::OnMouseButtonDown(MyGeometry, MouseEvent);
	FReply Reply = FReply::Unhandled();
	return Reply;
}

FReply SSquareTouchButton::OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	TouchEndPosition = MyGeometry.AbsoluteToLocal(MouseEvent.GetScreenSpacePosition());
	return SButton::OnMouseButtonUp(MyGeometry, MouseEvent);
}

FReply SSquareTouchButton::OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
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

void SSquareTouchButton::SetOnTouchMovedListener(const FSimpleDelegate& inListener)
{
	OnTouchMovedEvent = inListener;
}





void SSquareTouchButton::OnMouseLeave(const FPointerEvent& MouseEvent)
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

void SSquareTouchButton::Press()
{
	SButton::Press();
}

void SSquareTouchButton::Release()
{
	SButton::Release();
}

void SSquareTouchButton::ReleaseButtonAndCapture(FReply& InReply)
{
	if (InReply.GetMouseCaptor().IsValid() == false && HasMouseCapture())
	{
		InReply.ReleaseMouseCapture();
	}

	Release();
}

TSharedRef<SWidget> USquareTouchButton::RebuildWidget()
{
	TSharedPtr<SSquareTouchButton> touchButton = SNew(SSquareTouchButton)
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

void USquareTouchButton::SlateHandleTouchMoved()
{
	OnTouchMove.Broadcast();
}

FVector2D USquareTouchButton::GetTouchStartPosition()
{
	SSquareTouchButton* myTouchButton = static_cast<SSquareTouchButton*>(MyButton.Get());
	if (myTouchButton == nullptr)
		return FVector2D::ZeroVector;

	return myTouchButton->GetTouchStartPosition();
}

FVector2D USquareTouchButton::GetTouchMovePosition()
{
	SSquareTouchButton* myTouchButton = static_cast<SSquareTouchButton*>(MyButton.Get());
	if (myTouchButton == nullptr)
		return FVector2D::ZeroVector;

	return myTouchButton->GetTouchMovePosition();
}

FVector2D USquareTouchButton::GetTouchEndPosition()
{
	SSquareTouchButton* myTouchButton = static_cast<SSquareTouchButton*>(MyButton.Get());
	if (myTouchButton == nullptr)
		return FVector2D::ZeroVector;

	return myTouchButton->GetTouchEndPosition();
}