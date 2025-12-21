// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Components/Widget.h"
#include "GlobalEventListener.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGlobalEventListener : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class MODULARSTAGE_API IGlobalEventListener : public IInterface
{
	GENERATED_BODY()

		// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual bool IsCondition() { return true; }
};

//---------------------------------------------------------
UINTERFACE()
class UGlobalEventForWidget : public UGlobalEventListener
{
	GENERATED_BODY()
};

class IGlobalEventForWidget : public IGlobalEventListener
{
	GENERATED_BODY()

public:
	bool IsCondition() override
	{
		/*if (auto widget = Cast<UWidget>(this))
		{
			return WNDUILib::IsVisibleWithParent(widget);
		}*/
		return false;
	}
};