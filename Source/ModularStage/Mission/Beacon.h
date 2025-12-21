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

#if WITH_EDITOR
	friend class FBeaconDetails;
#endif

public:
	// Sets default values for this actor's properties
	ABeacon();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnSelectAssetPath(FString inPath);

	UFUNCTION(BlueprintCallable, Category = "Beacon")
	void UpdatePreview();

	UFUNCTION(BlueprintCallable, Category = "Beacon")
	void ClearPreview();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void LoadAndSpawnMesh();

	UFUNCTION()
	void OnMeshLoaded();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FSoftObjectPath MeshToLoad;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector VolumeSize = FVector(1000.0f, 1000.0f, 100.0f);

	UPROPERTY(EditAnywhere)
	TSoftClassPtr<AActor> TargetPrefab;

	UPROPERTY(EditAnywhere, meta = (MakeEditWidget = true))
	FTransform RelativeTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite);
	FString PrefabAssetPath;

	UPROPERTY(Transient)
	class UChildActorComponent* PreviewComp;

	// Timer handle for delayed NavMesh update
	FTimerHandle NavMeshUpdateTimerHandle;
};