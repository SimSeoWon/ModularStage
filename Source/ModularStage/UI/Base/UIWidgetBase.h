// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIWidgetBase.generated.h"

#define UI_BIND_BTN_CLICK_EVENT(btn, func) \
	if (btn != nullptr) \
	{ \
		if (!btn->OnClicked.IsAlreadyBound(this, func)) btn->OnClicked.AddDynamic(this, func); \
	}
#define UI_BIND_BTN_PRESS_EVENT(btn, func) \
	if (btn != nullptr) \
	{ \
		if (!btn->OnPressed.IsAlreadyBound(this, func)) btn->OnPressed.AddDynamic(this, func); \
	}
#define UI_BIND_BTN_RELEASE_EVENT(btn, func) \
	if (btn != nullptr) \
	{ \
		if (!btn->OnReleased.IsAlreadyBound(this, func)) btn->OnReleased.AddDynamic(this, func); \
	}

#define UI_BIND_BTN_HOVERED_EVENT(btn, func) \
	if (btn != nullptr) \
	{ \
		if (!btn->OnHovered.IsAlreadyBound(this, func)) btn->OnHovered.AddDynamic(this, func); \
	}

#define UI_BIND_BTN_UNHOVERED_EVENT(btn, func) \
	if (btn != nullptr) \
	{ \
		if (!btn->OnUnhovered.IsAlreadyBound(this, func)) btn->OnUnhovered.AddDynamic(this, func); \
	}


//------------------------------------------------------------------------
void UIPlaySound(UWidget* This, const FName& InSoundName);
#define UI_PLAYSOUND(soundName) UIPlaySound(this, soundName);
//------------------------------------------------------------------------

/**
 *
 */
UCLASS(Abstract) 
class MODULARSTAGE_API UUIWidgetBase : public UUserWidget
{
	GENERATED_BODY()
		// Function

};
