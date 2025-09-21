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

	UFUNCTION(BlueprintCallable, Category = "Level Generation")
	TArray<AActor*> GetActorsInFolder(const FString& inFolderPath);

	UFUNCTION(BlueprintCallable, Category = "Level Generation")
	void CreatePrefabFromActors(const TArray<AActor*>& Actors, const FString& NewBlueprintPath);

	/** 여러 액터로부터 컴포넌트만 수집하여 하나의 프리팹으로 생성합니다. */
	UFUNCTION(BlueprintCallable, Category = "Level Generation")
	void CreatePrefabFromCollectedComponents(const TArray<AActor*>& InActors, const FString& NewBlueprintPath);

	UPROPERTY(EditAnywhere, Category = "Prefab Generation | Settings")
	FString TargetFolderPath;

	UPROPERTY(EditAnywhere, Category = "Prefab Generation | Settings")
	FString NewPrefabPath;

	UFUNCTION(CallInEditor, Category = "Prefab Generation")
	void GeneratePrefabFromFolder();

#if WITH_EDITOR
	UFUNCTION(BlueprintCallable, Category = "Level Generation")
	void FocusEditorCameraOnActors(const TArray<AActor*>& inActors);
#endif
};