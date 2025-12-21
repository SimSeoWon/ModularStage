// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MissionPrefab.generated.h"

class UActorComponent_MissionTaskExecutor;
class UStaticMeshComponent;

/**
 * @brief 미션의 기본 단위가 되는 프리팹입니다.
 * 이 액터는 스테이지를 구성하는 하나의 '과제' 또는 '기믹'에 대한 데이터와 로직을 가집니다.
 * 예: 몬스터 스폰, 특정 지점 도달, 아이템 사용 등
 * 
 * MissionPrefabLevelScriptActor에 의해 관리됩니다.
 */
UCLASS()
class MODULARSTAGE_API AMissionPrefab : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMissionPrefab();

	/** 디테일 패널에서 이 프리팹을 개별 액터들로 분해하여 레벨에 배치합니다. */
	UFUNCTION(CallInEditor, Category = "Prefab")
	void UnpackForEditing();

    /** Opens the Mesh Generator utility widget to create a custom mesh for this prefab. */
    UFUNCTION(CallInEditor, Category = "Prefab | Mesh Generation")
    void OpenMeshGenerator();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mission")
	TObjectPtr<UActorComponent_MissionTaskExecutor> MissionTaskExecutor;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Prefab")
	TObjectPtr<UStaticMeshComponent> GeneratedMeshComponent;

	/** Defines the size of the simple box obstacle to generate. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Prefab | Mesh Generation")
	FVector ObstacleExtent;
};
