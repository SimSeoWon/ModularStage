// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "MissionPropComponent.h"
#include "MissionPropsComponent_Spawner.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MODULARSTAGE_API UMissionPropsComponent_Spawner : public USceneComponent, public IMissionPropComponent
{
	GENERATED_BODY()

#if WITH_EDITOR
	friend class FVisualizer_Spawner;
#endif

public:
	UMissionPropsComponent_Spawner();

	//~ Begin IMissionPropComponent Interface
	virtual void OnActivate() override;
	virtual void OnDeactivate() override;
	//~ End IMissionPropComponent Interface

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** 이 버튼이 디테일 패널에 나타납니다. */
	UFUNCTION(CallInEditor, Category = "Spawner")
	void ShowAsserBrowser();

protected:
	UPROPERTY(EditAnywhere, Category = "Spawner")
	FString ResourcesPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner", meta = (MakeEditWidget = true))
	TArray<FVector> SpawnPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner", meta = (MakeEditWidget = true))
	TArray<FVector> PatrolPoints;
};