#include "ModularStage_HexTouchButton.h"
#include "Components/ButtonSlot.h"
#include "ModularStage/ModularStage.h"

FReply SModularStage_HexTouchButton::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	TouchStartPosition = MyGeometry.AbsoluteToLocal(MouseEvent.GetScreenSpacePosition());
	SButton::OnMouseButtonDown(MyGeometry, MouseEvent);
	return FReply::Unhandled();
}

FReply SModularStage_HexTouchButton::OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	TouchEndPosition = MyGeometry.AbsoluteToLocal(MouseEvent.GetScreenSpacePosition());
	return SButton::OnMouseButtonUp(MyGeometry, MouseEvent);
}

FReply SModularStage_HexTouchButton::OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (IsPressed())
	{
		TouchMovePosition = MyGeometry.AbsoluteToLocal(MouseEvent.GetScreenSpacePosition());
		if (OnTouchMovedEvent.IsBound())
		{
			OnTouchMovedEvent.Execute();
		}
	}
	return FReply::Unhandled();
}

TSharedRef<SWidget> UModularStage_HexTouchButton::RebuildWidget()
{
	MyTouchButton = SNew(SModularStage_HexTouchButton)
		.OnClicked(BIND_UOBJECT_DELEGATE(FOnClicked, SlateHandleClicked))
		.OnPressed(BIND_UOBJECT_DELEGATE(FSimpleDelegate, SlateHandlePressed))
		.OnReleased(BIND_UOBJECT_DELEGATE(FSimpleDelegate, SlateHandleReleased))
		.OnHovered_UObject(this, &ThisClass::SlateHandleHovered)
		.OnUnhovered_UObject(this, &ThisClass::SlateHandleUnhovered)
		.ButtonStyle(&WidgetStyle)
		.ClickMethod(ClickMethod)
		.TouchMethod(TouchMethod)
		.PressMethod(PressMethod)
		.IsFocusable(IsFocusable);

	MyTouchButton->SetOnTouchMovedListener(BIND_UOBJECT_DELEGATE(FSimpleDelegate, SlateHandleTouchMoved));
	MyButton = MyTouchButton;

	if (GetChildrenCount() > 0)
	{
		Cast<UButtonSlot>(GetContentSlot())->BuildSlot(MyButton.ToSharedRef());
	}

	return MyButton.ToSharedRef();
}

void UModularStage_HexTouchButton::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	MyTouchButton.Reset();
}

void UModularStage_HexTouchButton::SlateHandleTouchMoved()
{
	OnTouchMove.Broadcast();
}

FVector2D UModularStage_HexTouchButton::GetTouchStartPosition() const
{
	return MyTouchButton.IsValid() ? MyTouchButton->GetTouchStartPosition() : FVector2D::ZeroVector;
}

FVector2D UModularStage_HexTouchButton::GetTouchMovePosition() const
{
	return MyTouchButton.IsValid() ? MyTouchButton->GetTouchMovePosition() : FVector2D::ZeroVector;
}

FVector2D UModularStage_HexTouchButton::GetTouchEndPosition() const
{
	return MyTouchButton.IsValid() ? MyTouchButton->GetTouchEndPosition() : FVector2D::ZeroVector;
}
