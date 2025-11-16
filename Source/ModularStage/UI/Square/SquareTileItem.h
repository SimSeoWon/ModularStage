// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Blueprint/IUserObjectListEntry.h>
#include "SquareTileItem.generated.h"

class UImage;
class UTextBlock;
class USquareTileDataBase;
/**
 * 
 */
UCLASS()
class MODULARSTAGE_API USquareTileItem : public UUserWidget, public IUserObjectListEntry
{
	GENERATED_BODY()
public:
	virtual void NativeOnListItemObjectSet(UObject* InObject) override;
	virtual void SetSelect(bool isSelect);
	virtual void Refresh();

public:
	UPROPERTY(meta = (BindWidget))
	UImage* Img_Square = nullptr;

	UPROPERTY(meta = (BindWidget))
	UImage* Img_Select = nullptr;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Txt_Index = nullptr;

private:
	TWeakObjectPtr<USquareTileDataBase> TileData;
};