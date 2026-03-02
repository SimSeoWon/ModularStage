#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "Widgets/Input/SButton.h"
#include "ModularStage_HexTouchButton.generated.h"

/**
 * 터치 이동 거리를 감지하기 위한 Slate 버튼 클래스
 */
class SModularStage_HexTouchButton : public SButton
{
public:
	virtual FReply OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;
	virtual FReply OnMouseMove(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent) override;

	FVector2D GetTouchStartPosition() const { return TouchStartPosition; }
	FVector2D GetTouchMovePosition() const { return TouchMovePosition; }
	FVector2D GetTouchEndPosition() const { return TouchEndPosition; }

	void SetOnTouchMovedListener(const FSimpleDelegate& InListener) { OnTouchMovedEvent = InListener; }

protected:
	FVector2D TouchStartPosition = FVector2D::ZeroVector;
	FVector2D TouchMovePosition = FVector2D::ZeroVector;
	FVector2D TouchEndPosition = FVector2D::ZeroVector;

	FSimpleDelegate OnTouchMovedEvent;
};

/**
 * 육각형 타일 뷰에서 드래그 및 터치 입력을 처리하기 위한 확장 버튼 컴포넌트
 */
UCLASS()
class MODULARSTAGE_API UModularStage_HexTouchButton : public UButton
{
	GENERATED_BODY()

public:
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

	FVector2D GetTouchStartPosition() const;
	FVector2D GetTouchMovePosition() const;
	FVector2D GetTouchEndPosition() const;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTouchMoveEvent);
	UPROPERTY(BlueprintAssignable, Category = "HexTouch")
	FOnTouchMoveEvent OnTouchMove;

protected:
	void SlateHandleTouchMoved();

	TSharedPtr<SModularStage_HexTouchButton> MyTouchButton;
};
