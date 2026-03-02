#pragma once

#include "CoreMinimal.h"
#include "Components/PrimitiveComponent.h"
#include "HexGridVisualComponent.generated.h"

struct FHexTileConfig;

/**
 * AMissionPrefab의 Septet 클러스터를 에디터 뷰포트에서 상시 렌더링하기 위한 컴포넌트
 */
UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MODULARSTAGE_API UHexGridVisualComponent : public UPrimitiveComponent
{
	GENERATED_BODY()

public:
	UHexGridVisualComponent();

	/** 상위 Prefab으로부터 타일 설정 데이터를 전달받아 갱신합니다. */
	void UpdateTileConfigs(const TArray<FHexTileConfig>& InConfigs);

protected:
	// UPrimitiveComponent interfaces
	virtual FPrimitiveSceneProxy* CreateSceneProxy() override;
	virtual FBoxSphereBounds CalcBounds(const FTransform& LocalToWorld) const override;

private:
	UPROPERTY()
	TArray<FHexTileConfig> TileConfigs;

	friend class FHexGridSceneProxy;
};
