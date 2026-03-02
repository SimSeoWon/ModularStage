// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent_MissionTaskExecutor.h"
#include "ModularStage/Mission/TaskSystem/MissionTaskExecutor.h"
#include "ModularStage/Mission/MissionPrefab.h" 
#include "ModularStage/Table/MissionTaskTable.h"
#include "ModularStage/Manager/Manager_Mission.h"

// Sets default values for this component's properties
UActorComponent_MissionTaskExecutor::UActorComponent_MissionTaskExecutor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void UActorComponent_MissionTaskExecutor::BeginPlay()
{
	Super::BeginPlay();
}

void UActorComponent_MissionTaskExecutor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (IsValid(Executor))
	{
		Executor->Shutdown();
	}

	Super::EndPlay(EndPlayReason);
}

// Called every frame
void UActorComponent_MissionTaskExecutor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UActorComponent_MissionTaskExecutor::SetData(const TArray<FMissionTaskInfo>& TaskList)
{
	if (Executor == nullptr)
	{
		Executor = NewObject<UMissionTaskExecutor>(this);
	}

	if (IsValid(Executor))
	{
		return Executor->InitializeFromTaskList(TaskList);
	}
	return false;
}

void UActorComponent_MissionTaskExecutor::SetExecutionInfo(EMissionType InType, int32 InOrder)
{
	if (IsValid(Executor))
	{
		Executor->SetExecutionInfo(InType, InOrder);
	}
}

void UActorComponent_MissionTaskExecutor::RegisterWithManager()
{
	if (UManager_Mission* TaskManager = UManager_Mission::Get())
	{
		TaskManager->RegisterExecutor(Executor);
	}
}
