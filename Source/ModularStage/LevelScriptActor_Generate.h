// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "LevelScriptActor_Generate.generated.h"

class AActor;

UCLASS()
class MODULARSTAGE_API ALevelScriptActor_Generate : public ALevelScriptActor
{
	GENERATED_BODY()

public:
	ALevelScriptActor_Generate();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
//
//#if WITH_EDITOR
//	UFUNCTION(BlueprintCallable, Category = "Level Generation")
//	void FocusEditorCameraOnActors(const TArray<AActor*>& inActors);
//#endif
};