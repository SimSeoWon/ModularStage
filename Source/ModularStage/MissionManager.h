// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MissionManager.generated.h"

/**
 * 
 */
UCLASS()
class MODULARSTAGE_API UMissionManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
    // USubsystem interface begin
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;
    // USubsystem interface end
};
