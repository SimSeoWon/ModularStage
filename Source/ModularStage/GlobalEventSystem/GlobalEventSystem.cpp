// Fill out your copyright notice in the Description page of Project Settings.


#include "GlobalEventSystem.h"
#include "Interfaces/GlobalEventListener.h"
#include "Components/Widget.h"

TSharedPtr<FGlobalEventSystem> FGlobalEventSystem::PrivateHandler = MakeShareable(new FGlobalEventSystem());