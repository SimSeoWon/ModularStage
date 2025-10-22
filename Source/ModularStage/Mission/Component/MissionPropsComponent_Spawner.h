// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/PrimitiveComponent.h"
#include "PrimitiveSceneProxy.h"
#include "MissionPropComponent.h"
#include "Engine/Texture2D.h" // UTexture2D를 사용하기 위해 Include 추가
#include "MissionPropsComponent_Spawner.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MODULARSTAGE_API UMissionPropsComponent_Spawner : public UPrimitiveComponent, public IMissionPropComponent
{
	GENERATED_BODY()

#if WITH_EDITOR
	friend class FVisualizer_Spawner;
#endif

	friend class FSpawnerSceneProxy;

public:
	UMissionPropsComponent_Spawner();

	virtual FPrimitiveSceneProxy* CreateSceneProxy() override;

	// !! 중요: FBoxSphereBounds GetBounds() const override; // <- UPrimitiveComponent는 이 함수가 없음
	// !! FBoxSphereBounds CalcBounds(const FTransform& LocalToWorld) const override; 를 구현해야 함
	virtual FBoxSphereBounds CalcBounds(const FTransform& LocalToWorld) const override;

	//~ Begin IMissionPropComponent Interface
	virtual void OnActivate() override;
	virtual void OnDeactivate() override;
	//~ End IMissionPropComponent Interface

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	
	UFUNCTION(CallInEditor, Category = "Spawner")
	void ShowAsserBrowser();

protected:

	/** 컴포넌트를 뷰포트에서 나타낼 아이콘입니다. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visual")
	TObjectPtr<UTexture2D> SpriteIcon;

	UPROPERTY(EditAnywhere, Category = "Spawner")
	FString ResourcesPath;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner", meta = (MakeEditWidget = true))
	TArray<FVector> SpawnPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner", meta = (MakeEditWidget = true))
	TArray<FVector> PatrolPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner", meta = (MakeEditWidget = true))
	bool IsShowDebug = true;
};