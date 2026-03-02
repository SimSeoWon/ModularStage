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
	friend class UPrefabEditorWidget;
#endif

	friend class FSpawnerSceneProxy;

public:
	UMissionPropsComponent_Spawner();

	virtual FPrimitiveSceneProxy* CreateSceneProxy() override;

	// !! 중요: FBoxSphereBounds GetBounds() const override; // <- UPrimitiveComponent는 이 함수가 없음
	// !! FBoxSphereBounds CalcBounds(const FTransform& LocalToWorld) const override; 를 구현해야 함
	virtual FBoxSphereBounds CalcBounds(const FTransform& LocalToWorld) const override;

	//~ Begin IMissionPropComponent Interface
	virtual void SetGuid(const FGuid& InGuid) override;
	virtual FGuid GetGuid() const override;
	virtual void OnActivate() override;
	virtual void OnDeactivate() override;
	virtual void SetHexTileIndex(int32 InIndex) override { HexTileIndex = InIndex; }
	virtual int32 GetHexTileIndex() const override { return HexTileIndex; }
	//~ End IMissionPropComponent Interface

	virtual void PostInitProperties() override;

	void SetResourcesPath(const FString& InPath) { ResourcesPath = InPath; }

protected:
	/** 이 컴포넌트가 배치될 헥스 타일 인덱스 (0~6) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "MissionProp | HexGrid")
	int32 HexTileIndex = 0;

private:
	/** A unique identifier for this component instance. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="MissionProp", meta=(AllowPrivateAccess="true"))
	FGuid ComponentGuid;

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	
	UFUNCTION(CallInEditor, Category = "Spawner")
	void ShowAsserBrowser();

	UFUNCTION()
	void OnSelectAssetPath(FString inPath);

protected:

	/** 컴포넌트를 뷰포트에서 나타낼 아이콘입니다. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visual")
	TObjectPtr<UTexture2D> SpriteIcon;

	UPROPERTY(EditAnywhere, Category = "Spawner")
	FString ResourcesPath;

	/** 이 스포너가 몬스터를 생성할 타일 인덱스 목록 (0~6) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner | HexGrid")
	TArray<int32> SpawnTileIndices;

	/** 몬스터가 순찰할 타일 인덱스 목록 (0~6) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner | HexGrid")
	TArray<int32> PatrolTileIndices;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawner")
	bool IsShowDebug = true;
};