// Fill out your copyright notice in the Description page of Project Settings.


#include "SquareTileItem.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanelSlot.h"


#include "SquareTileDataBase.h"

void USquareTileItem::NativeOnListItemObjectSet(UObject* InObject)
{
	if (false == IsValid(InObject))
		return;

	TileData = Cast<USquareTileDataBase>(InObject);
	if (TileData.IsValid())
	{
		TileData->SetTileObject(this);
	}
	Refresh();
}

void USquareTileItem::Refresh()
{
	if (false == TileData.IsValid())
		return;

	Txt_Index->SetText(FText::FromString(FString::Printf(TEXT("%d"), TileData->Index)));

	float size = TileData->GetSize();
	UCanvasPanelSlot* slot = Cast<UCanvasPanelSlot>(Img_Square->Slot);
	if (IsValid(slot))
	{
		slot->SetSize(FVector2D(size, size));
	}

	// Set color based on movability
	if (TileData->IsMovable())
	{
		Img_Square->SetColorAndOpacity(FLinearColor::Green); // Movable tiles are green
	}
	else
	{
		Img_Square->SetColorAndOpacity(FLinearColor::Red); // Immovable tiles are red
	}
}