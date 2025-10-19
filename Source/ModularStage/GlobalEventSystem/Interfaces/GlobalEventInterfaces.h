// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Templates/SharedPointer.h"
#include "UObject/WeakObjectPtrTemplates.h"
#include "Delegates/DelegateCombinations.h"
#include "GlobalEventListener.h"
#include "GlobalEventinterfaces.generated.h"

UINTERFACE()
class UEventContentsOpen : public UGlobalEventForWidget
{
	GENERATED_BODY()
};

class IEventContentsOpen : public IGlobalEventForWidget
{
	GENERATED_BODY()
public:
};

UINTERFACE()
class UEventCharacterStatValue : public UGlobalEventForWidget
{
	GENERATED_BODY()
};

class IEventCharacterStatValue : public IGlobalEventForWidget
{
	GENERATED_BODY()
public:
};



