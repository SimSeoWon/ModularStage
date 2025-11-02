 // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"
#include "NavigationSystem.h"
#include "Components/StaticMeshComponent.h"
#include "Beacon.generated.h"

UCLASS()
class MODULARSTAGE_API ABeacon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABeacon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Beacon")
	FSoftObjectPath MeshToLoad;

protected:
	UFUNCTION()
	void LoadAndSpawnMesh();

	UFUNCTION()
	void OnMeshLoaded();

	// Timer handle for delayed NavMesh update
	FTimerHandle NavMeshUpdateTimerHandle;
};