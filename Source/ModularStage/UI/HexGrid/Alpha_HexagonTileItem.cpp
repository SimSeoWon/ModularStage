// Fill out your copyright notice in the Description page of Project Settings.


#include "Alpha_HexagonTileItem.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanelSlot.h"


#include "Alpha_HexagonTileDataBase.h"

void UAlpha_HexagonTileItem::NativeOnListItemObjectSet(UObject* InObject)
{
	if (false == IsValid(InObject))
		return;

	TileData = Cast<UAlpha_HexagonTileDataBase>(InObject);
	TileData->SetTileObject(this);
	Refresh();
}

void UAlpha_HexagonTileItem::Refresh()
{
	if (false == IsValid(TileData))
		return;

	Txt_Index->SetText(FText::FromString(FString::Printf(TEXT("%d"), TileData->Index)));

	float size = TileData->GetSize();
	UCanvasPanelSlot* slot = Cast<UCanvasPanelSlot>(Img_Hexagon->Slot);
	if (IsValid(slot))
	{
		slot->SetSize(FVector2D(size * 2, size * 2));
	}

	slot = Cast<UCanvasPanelSlot>(Img_Select->Slot);
	if (IsValid(slot))
	{
		slot->SetSize(FVector2D(size * 2, size * 2));
	}

	bool isSelect = TileData->GetIsSelect();
	SetSelect(isSelect);

	uint8 idx = (uint8)TileData->HeightType;
	if (ColorList.IsValidIndex(idx)) 
	{
		Img_Hexagon->SetColorAndOpacity(ColorList[idx]);
	}
}

void UAlpha_HexagonTileItem::SetSelect(bool isSelect)
{
	Img_Select->SetVisibility(isSelect ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
}