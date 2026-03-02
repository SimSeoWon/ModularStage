// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SUserWidget.h"
#include "Components/Button.h"
#include <Widgets/Input/SButton.h>
#include <Templates/SharedPointer.h>
#include "Alpha_HexagonTouchButton.generated.h"

class SAlpha_HexagonTouchButton : public SButton
{
public:
	SAlpha_HexagonTouchButton() {}
	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual void OnMouseLeave(const FPointerEvent& MouseEvent) override;

	virtual void Press() override;
	virtual void Release() override;

	void ReleaseButtonAndCapture(FReply& InReply);

	FVector2D GetTouchStartPosition() { return TouchStartPosition; }
	FVector2D GetTouchEndPosition() { return TouchEndPosition; }
	FVector2D GetTouchMovePosition() { return TouchMovePosition; }

	void SetOnTouchMovedListener(const FSimpleDelegate& inListener);

protected:
	FVector2D DragDist = FVector2D::ZeroVector;

	FVector2D TouchStartPosition = FVector2D::ZeroVector;

	FVector2D TouchMovePosition = FVector2D::ZeroVector;

	FVector2D TouchEndPosition = FVector2D::ZeroVector;

	FSimpleDelegate	OnTouchMovedEvent;
};

UCLASS()
class UAlpha_HexagonTouchButton : public UButton
{
	GENERATED_BODY()

protected:
	void SlateHandleTouchMoved();

public:
	virtual TSharedRef<SWidget> RebuildWidget() override;

	FVector2D GetTouchStartPosition();
	FVector2D GetTouchMovePosition();
	FVector2D GetTouchEndPosition();

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTouchMoveEvent);
	UPROPERTY(BlueprintAssignable, Category = "Button|Event")
	FOnTouchMoveEvent  OnTouchMove;
};