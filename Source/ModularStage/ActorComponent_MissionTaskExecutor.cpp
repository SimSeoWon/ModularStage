// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponent_MissionTaskExecutor.h"


// Sets default values for this component's properties
UActorComponent_MissionTaskExecutor::UActorComponent_MissionTaskExecutor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UActorComponent_MissionTaskExecutor::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

// Called every frame
void UActorComponent_MissionTaskExecutor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
