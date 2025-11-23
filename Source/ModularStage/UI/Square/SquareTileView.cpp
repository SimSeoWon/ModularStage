// Fill out your copyright notice in the Description page of Project Settings.


#include "SquareTileView.h"
#include "Components/Image.h"
#include "Components/Spacer.h"
#include "Components/SizeBox.h"
#include "Components/PanelWidget.h"
#include "Components/CanvasPanelSlot.h"
#include "SquareTileDataBase.h"
#include "SquareTileItem.h"
#include "SquareTouchButton.h"

void USquareTileView::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(TouchButton))
	{
		TouchButton->OnClicked.AddDynamic(this, &USquareTileView::OnClicked);
		TouchButton->OnPressed.AddDynamic(this, &USquareTileView::OnClicked);
		TouchButton->OnTouchMove.AddDynamic(this, &USquareTileView::OnTouchMove);
	}
}

void USquareTileView::NativeDestruct()
{
	Super::NativeDestruct();
}

void USquareTileView::SynchronizeProperties()
{
	Super::SynchronizeProperties();


#if WITH_EDITORONLY_DATA
	// UI   
	if (false == IsDesignTime())
		return;

	if (false == Edit_Preview)
		return;

	if (0 >= Edit_Count)
		return;

	TArray<USquareTileDataBase*> arrSquareTile;

	for (int32 i = 0; i < Edit_Count; i++)
	{
		USquareTileDataBase* info = NewObject<USquareTileDataBase>();
		if (false == IsValid(info))
			continue;

		arrSquareTile.Add(info);
	}

	SetTileListItems(arrSquareTile);

#endif
}

void USquareTileView::SetTileListItems(float inTileSize, int32 inColumn, int32 inCount, float inScale/**= 100.0f*/)
{
	TileSize = inTileSize;
	Scale = inScale;
	Column = inColumn;

	float correctedSize = TileSize * Scale;

	TileHalfHeight = correctedSize / 2;
	TileHalfWidth = correctedSize / 2;

	TArray<USquareTileDataBase*> arrSquareTile;

	for (int32 i = 0; i < inCount; i++)
	{
		USquareTileDataBase* info = NewObject<USquareTileDataBase>();
		if (false == IsValid(info))
			continue;

		info->Index = i;

		FVector2D coordinates(i % Column, i / Column);
		info->Init(coordinates, correctedSize, Scale);

		arrSquareTile.Add(info);
	}

	SetTileListItems(arrSquareTile);
}

void USquareTileView::SetTileListItems(const TArray<USquareTileDataBase*>& inListItems)
{
	ClearSquareTileList();
	TileDataList.Append(inListItems);

	float correctedSize = TileSize * Scale;

	TileHalfHeight = correctedSize / 2;
	TileHalfWidth = correctedSize / 2;
	int32 tileCount = TileDataList.Num();

	FMargin margin{ 0, 0, 0, 0 };

	for (int32 i = 0; i < tileCount; i++)
	{
		USquareTileDataBase* const tileData = TileDataList[i];
		if (!IsValid(tileData))
			continue;

		FVector2D coordinates(i % Column, i / Column);
		tileData->Init(coordinates, correctedSize, Scale);

		USquareTileItem* tileWidget = CreateWidget<USquareTileItem>(GetWorld(), SquareTileAsset.LoadSynchronous());
		if (IsValid(tileWidget))
		{
			TilePanel->AddChild(tileWidget);
			tileWidget->SetRenderTranslation(tileData->Center);
			UCanvasPanelSlot* slot = Cast<UCanvasPanelSlot>(tileWidget->Slot);
			if (IsValid(slot))
				slot->SetOffsets(margin);

			tileWidget->NativeOnListItemObjectSet(tileData);
			SquareTileList.Emplace(tileWidget);
		}
	}

	if (Column >= tileCount)
	{
		TotalWidth = tileCount * TileSize;
		TotalHeight = TileSize;
	}
	else
	{
		int32 rowCount = FMath::CeilToInt((float)tileCount / (float)Column);
		TotalWidth = Column * TileSize;
		TotalHeight = rowCount * TileSize;
	}

	UCanvasPanelSlot* canvasSlot = Cast<UCanvasPanelSlot>(RootPanel->Slot);
	if (IsValid(canvasSlot))
		canvasSlot->SetSize(FVector2D(TotalWidth, TotalHeight));

}

void USquareTileView::SetScale(float inScale)
{
	Scale = inScale;

	float correctedSize = TileSize * Scale;

	TileHalfHeight = correctedSize / 2;
	TileHalfWidth = correctedSize / 2;
	int32 tileCount = TileDataList.Num();

	FMargin margin{ 0, 0, 0, 0 };

	for (int32 i = 0; i < tileCount; i++)
	{
		USquareTileDataBase* const tileData = TileDataList[i];
		if (!IsValid(tileData))
			continue;

		FVector2D coordinates(i % Column, i / Column);
		tileData->Init(coordinates, correctedSize, Scale);

		TWeakObjectPtr< USquareTileItem> tileWidget = SquareTileList[i];
		if (false == tileWidget.IsValid())
			continue;

		tileWidget->SetRenderTranslation(tileData->Center);
		UCanvasPanelSlot* slot = Cast<UCanvasPanelSlot>(tileWidget->Slot);
		if (IsValid(slot))
			slot->SetOffsets(margin);

		tileWidget->NativeOnListItemObjectSet(tileData);
	}

	if (Column >= tileCount)
	{
		TotalWidth = tileCount * TileSize;
		TotalHeight = TileSize;
	}
	else
	{
		int32 rowCount = FMath::CeilToInt((float)tileCount / (float)Column);
		TotalWidth = Column * TileSize;
		TotalHeight = rowCount * TileSize;
	}

	UCanvasPanelSlot* canvasSlot = Cast<UCanvasPanelSlot>(RootPanel->Slot);
	if (IsValid(canvasSlot))
		canvasSlot->SetSize(FVector2D(TotalWidth, TotalHeight));
}

USquareTileDataBase* USquareTileView::GetTileListItem(int32 inIndex)
{
	if (false == TileDataList.IsValidIndex(inIndex))
	{
		return nullptr;
	}

	return TileDataList[inIndex];
}

void USquareTileView::ClearSquareTileList()
{
	for (int32 i = 0; i < SquareTileList.Num(); i++)
	{
		TilePanel->RemoveChild(SquareTileList[i].Get());
	}

	TileDataList.Reset();
	SquareTileList.Reset();
}

void USquareTileView::OnTouchMove()
{
	/*if (false == IsValid(TouchButton))
		return;

	FVector2D movePosition = TouchButton->GetTouchMovePosition();
	OnTouchStart(movePosition);*/
}

void USquareTileView::OnClicked()
{
	if (false == IsValid(TouchButton))
		return;

	FVector2D startPosition = TouchButton->GetTouchStartPosition();
	OnTouchStart(startPosition);
}

void USquareTileView::OnTileClicked(int32 inIndex)
{
	if (TileDataList.IsValidIndex(inIndex))
	{
		USquareTileDataBase* TileData = TileDataList[inIndex];
		if (IsValid(TileData))
		{
			TileData->SetMovable(!TileData->IsMovable());
			TileData->Refresh();

			OnTileToggled.ExecuteIfBound(TileData);
		}
	}
}

void USquareTileView::OnTouchStart(const FVector2D inPostion)
{
	//UE_LOG(LogTemp, Warning, TEXT("USquareTileView::OnTouchStart() - inPostion(%f, %f)"), inPostion.X, inPostion.Y);
	int32 selectRow = FMath::FloorToInt(inPostion.Y / (TileSize * Scale));
	int32 selectColumn = FMath::FloorToInt(inPostion.X / (TileSize * Scale));
	int32 selectIndex = 0;

	selectIndex = selectRow * Column + selectColumn;

	if (TileDataList.IsValidIndex(selectIndex))
	{
		if (TileDataList[selectIndex]->CollisionCheck(inPostion))
		{
			OnTileClicked(selectIndex);
		}
	}
}
