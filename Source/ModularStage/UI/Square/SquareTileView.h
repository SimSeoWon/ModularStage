// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Blueprint/UserWidget.h"
#include "SquareTileView.generated.h"

class UImage;
class UPanelWidget;
class USquareTileDataBase;
class USquareTileItem;
class USquareTouchButton;


/**
 * 
 */
UCLASS()
class MODULARSTAGE_API USquareTileView : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	void SetScale(float inScale);

	void SetTileListItems(float inTileSize, int32 inColumn, int32 inCount, float inScale = 1.0f);

	void SetTileListItems(const TArray<USquareTileDataBase*>& InListItems);

	USquareTileDataBase* GetTileListItem(int32 inIndex);


	UFUNCTION(BlueprintCallable)
	void ClearSquareTileList();

	UFUNCTION()
	void OnTouchStart(const FVector2D inPostion);
	void SetSelectTile(int32 inIndex);

	UFUNCTION()
	void OnClicked();

	UFUNCTION()
	void OnTouchMove();

protected:
	virtual void SynchronizeProperties() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSoftClassPtr<USquareTileItem> SquareTileAsset;

	UPROPERTY(meta = (BindWidget))
	USquareTouchButton* TouchButton = nullptr;

	UPROPERTY(meta = (BindWidget))
	UPanelWidget* RootPanel = nullptr;

	UPROPERTY(meta = (BindWidget))
	UPanelWidget* TilePanel = nullptr;

#pragma region standards info
	UPROPERTY(EditAnywhere)
	float TileSize = 0.0f;
	float Scale = 1.0f;

	float TileHeight = 0.0f;
	float TileWidth = 0.0f;
	float TileHalfHeight = 0.0f;
	float TileHalfWidth = 0.0f;
	float TotalWidth = 0.0f;
	float TotalHeight = 0.0f;
#pragma endregion

	UPROPERTY(EditAnywhere)
	int32 Column = 1; 

	UPROPERTY(Transient)
	TArray<USquareTileDataBase*> TileDataList;

	UPROPERTY(Transient)
	USquareTileDataBase* SelectTileData = nullptr;

	TArray<TWeakObjectPtr<USquareTileItem>> SquareTileList;
	DECLARE_DELEGATE_OneParam(FSelectSquareTileDelegate, const FVector2D)
	FSelectSquareTileDelegate OnSelectSquareTile;

#if WITH_EDITORONLY_DATA
private:
	UPROPERTY(EditAnywhere)
	bool Edit_Preview = false;

	UPROPERTY(EditAnywhere)
	int32 Edit_Count = 10;
#endif
	
};
