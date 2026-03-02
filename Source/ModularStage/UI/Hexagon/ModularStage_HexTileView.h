#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ModularStage_HexTileView.generated.h"

class UModularStage_HexTileDataBase;
class UModularStage_HexTileItem;
class UModularStage_HexTouchButton;

/**
 * 육각형 타일들을 그리드 형태로 화면에 표시하는 위젯
 */
UCLASS()
class MODULARSTAGE_API UModularStage_HexTileView : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

	void SetScale(float InScale);
	void SetTileListItems(float InTileSize, int32 InColumn, int32 InCount, float InScale = 1.0f);
	void SetTileListItems(const TArray<UModularStage_HexTileDataBase*>& InListItems);

	UModularStage_HexTileDataBase* GetTileListItem(int32 InIndex);
	void ClearHexagonTileList();

	void SetSelectTile(int32 InIndex);

	UFUNCTION()
	void OnTouchStart(const FVector2D InPosition);

	UFUNCTION()
	void OnClicked();

	UFUNCTION()
	void OnTouchMove();

protected:
	virtual void SynchronizeProperties() override;
	bool NearestTileCollisionCheck(FVector2D InPosition, FVector2D InNeighbors);

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HexGrid")
	TSoftClassPtr<UModularStage_HexTileItem> HexagonTileAsset;

	UPROPERTY(meta = (BindWidget))
	UModularStage_HexTouchButton* TouchButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* RootPanel = nullptr;

	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel* TilePanel = nullptr;

	// --- 그리드 규격 정보 ---
	float TileSize = 100.0f;
	float Scale = 1.0f;
	float TileHeight = 0.0f;
	float TileWidth = 0.0f;
	float TileHalfHeight = 0.0f;
	float TileQuarterHeight = 0.0f;
	float TileHalfWidth = 0.0f;
	float TotalWidth = 0.0f;
	float TotalHeight = 0.0f;

	int32 Column = 1;

	UPROPERTY(EditAnywhere, Category = "HexGrid")
	bool bUseMultipleSelect = false;

	UPROPERTY(Transient)
	TArray<UModularStage_HexTileDataBase*> TileDataList;

	UPROPERTY(Transient)
	UModularStage_HexTileDataBase* SelectTileData = nullptr;

	TArray<TWeakObjectPtr<UModularStage_HexTileItem>> HexagonTileList;

	DECLARE_DELEGATE_OneParam(FOnSelectHexagonTile, const FVector2D)
	FOnSelectHexagonTile OnSelectHexagonTile;

#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, Category = "Editor")
	bool bEdit_Preview = false;

	UPROPERTY(EditAnywhere, Category = "Editor")
	int32 Edit_Count = 10;
#endif
};

/**
 * 개별 타일 위젯의 베이스 클래스 (C++ 로직 담당)
 */
UCLASS()
class MODULARSTAGE_API UModularStage_HexTileItem : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent)
	void Refresh(); // 데이터에 따라 위젯 외형(색상 등) 업데이트

	UFUNCTION(BlueprintImplementableEvent)
	void NativeOnListItemObjectSet(UObject* InObject);
};
