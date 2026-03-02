// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ModularStage/Table/TableEnum.h"

#include "ActorComponent_MissionTaskExecutor.generated.h"

class UMissionTaskExecutor;
struct FMissionTaskInfo;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MODULARSTAGE_API UActorComponent_MissionTaskExecutor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UActorComponent_MissionTaskExecutor();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	bool SetData(const TArray<FMissionTaskInfo>& TaskList);
	void SetExecutionInfo(EMissionType InType, int32 InOrder);
	void RegisterWithManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;


private:
	UPROPERTY()
	TObjectPtr<UMissionTaskExecutor> Executor = nullptr;
};
