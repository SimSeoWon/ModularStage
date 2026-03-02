// Fill out your copyright notice in the Description page of Project Settings.


#include "Alpha_HexagonTileView.h"
#include "Alpha_HexagonTouchWidget.h"
#include "Components/Image.h"
#include "Components/Spacer.h"
#include "Components/SizeBox.h"
#include "Components/PanelWidget.h"
#include "Components/CanvasPanelSlot.h"
#include "Alpha_HexagonTileDataBase.h"
#include "Alpha_HexagonTileItem.h"
#include "Alpha_HexagonTouchButton.h"

void UAlpha_HexagonTileView::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(TouchButton))
	{
		TouchButton->OnClicked.AddDynamic(this, &UAlpha_HexagonTileView::OnClicked);
		TouchButton->OnPressed.AddDynamic(this, &UAlpha_HexagonTileView::OnClicked);
		TouchButton->OnTouchMove.AddDynamic(this, &UAlpha_HexagonTileView::OnTouchMove);
	}
}

void UAlpha_HexagonTileView::NativeDestruct()
{
	Super::NativeDestruct();
}

void UAlpha_HexagonTileView::SynchronizeProperties()
{
	Super::SynchronizeProperties();


#if WITH_EDITORONLY_DATA
	// UI 디자인 시점 지원
	if (false == IsDesignTime())
		return;

	if (false == Edit_Preview)
		return;

	if (0 >= Edit_Count)
		return;

	TArray<UAlpha_HexagonTileDataBase*> arrHexagonTile;

	for (int32 i = 0; i < Edit_Count; i++)
	{
		UAlpha_HexagonTileDataBase* info = NewObject<UAlpha_HexagonTileDataBase>();
		if (false == IsValid(info))
			continue;

		arrHexagonTile.Add(info);
	}

	SetTileListItems(arrHexagonTile);

#endif
}

void UAlpha_HexagonTileView::SetTileListItems(float inTileSize, int32 inColumn, int32 inCount, float inScale/**= 100.0f*/)
{
	TileSize = inTileSize;
	Scale = inScale;
	Column = inColumn;

	float correctedSize = TileSize * Scale;

	TileHeight = 2 * correctedSize;
	TileWidth = FMath::Sqrt(3.0f) * correctedSize;
	TileHalfHeight = correctedSize;
	TileQuarterHeight = TileHalfHeight / 2;
	TileHalfWidth = TileWidth / 2;

	TArray<UAlpha_HexagonTileDataBase*> arrHexagonTile;

	for (int32 i = 0; i < inCount; i++)
	{
		UAlpha_HexagonTileDataBase* info = NewObject<UAlpha_HexagonTileDataBase>();
		if (false == IsValid(info))
			continue;

		info->Index = i;

		FVector2D coordinates(i % Column, i / Column);
		info->Init(coordinates, correctedSize, TileWidth, TileHeight, Scale);

		arrHexagonTile.Add(info);
	}

	SetTileListItems(arrHexagonTile);
}

void UAlpha_HexagonTileView::SetTileListItems(const TArray<UAlpha_HexagonTileDataBase*>& inListItems)
{
	ClearHexagonTileList();
	TileDataList.Append(inListItems);

	float correctedSize = TileSize * Scale;

	TileHeight = 2 * correctedSize;
	TileWidth = FMath::Sqrt(3.0f) * correctedSize;
	TileHalfHeight = correctedSize;
	TileQuarterHeight = TileHalfHeight / 2;
	TileHalfWidth = TileWidth / 2;
	int32 tileCount = TileDataList.Num();

	FMargin margin{ 0, 0, 0, 0 };

	for (int32 i = 0; i < tileCount; i++)
	{
		UAlpha_HexagonTileDataBase* const tileData = TileDataList[i];
		if (!IsValid(tileData))
			continue;

		FVector2D coordinates(i % Column, i / Column);
		tileData->Init(coordinates, correctedSize, TileWidth, TileHeight, Scale);

		UAlpha_HexagonTileItem* tileWidget = CreateWidget<UAlpha_HexagonTileItem>(GetWorld(), HexagonTileAsset.LoadSynchronous());
		if (IsValid(tileWidget))
		{
			TilePanel->AddChild(tileWidget);
			tileWidget->SetRenderTranslation(tileData->Center);
			UCanvasPanelSlot* slot = Cast<UCanvasPanelSlot>(tileWidget->Slot);
			if (IsValid(slot))
				slot->SetOffsets(margin);

			tileWidget->NativeOnListItemObjectSet(tileData);
			HexagonTileList.Emplace(tileWidget);
		}
	}

	if (Column >= tileCount)
	{
		TotalWidth = tileCount * TileWidth;
		TotalHeight = ((TileHeight - TileQuarterHeight) * 1) + TileQuarterHeight;
	}
	else
	{
		int32 rowCount = tileCount / Column;
		TotalWidth = Column * TileWidth;

		TotalWidth += TileHalfWidth;

		if (tileCount % Column > 0) // 나머지값 체크.
			rowCount++;

		TotalHeight = ((TileHeight - TileQuarterHeight) * rowCount) + TileQuarterHeight;
	}

	UCanvasPanelSlot* canvasSlot = Cast<UCanvasPanelSlot>(RootPanel->Slot);
	if (IsValid(canvasSlot))
		canvasSlot->SetSize(FVector2D(TotalWidth, TotalHeight));

}

void UAlpha_HexagonTileView::SetScale(float inScale)
{
	Scale = inScale;

	float correctedSize = TileSize * Scale;

	TileHeight = 2 * correctedSize;
	TileWidth = FMath::Sqrt(3.0f) * correctedSize;
	TileHalfHeight = correctedSize;
	TileQuarterHeight = TileHalfHeight / 2;
	TileHalfWidth = TileWidth / 2;
	int32 tileCount = TileDataList.Num();

	FMargin margin{ 0, 0, 0, 0 };

	for (int32 i = 0; i < tileCount; i++)
	{
		UAlpha_HexagonTileDataBase* const tileData = TileDataList[i];
		if (!IsValid(tileData))
			continue;

		FVector2D coordinates(i % Column, i / Column);
		tileData->Init(coordinates, correctedSize, TileWidth, TileHeight, Scale);

		TWeakObjectPtr< UAlpha_HexagonTileItem> tileWidget = HexagonTileList[i];
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
		TotalWidth = tileCount * TileWidth;
		TotalHeight = ((TileHeight - TileQuarterHeight) * 1) + TileQuarterHeight;
	}
	else
	{
		int32 rowCount = tileCount / Column;
		TotalWidth = Column * TileWidth;

		TotalWidth += TileHalfWidth;

		if (tileCount % Column > 0) // 나머지값 체크.
			rowCount++;

		TotalHeight = ((TileHeight - TileQuarterHeight) * rowCount) + TileQuarterHeight;
	}

	UCanvasPanelSlot* canvasSlot = Cast<UCanvasPanelSlot>(RootPanel->Slot);
	if (IsValid(canvasSlot))
		canvasSlot->SetSize(FVector2D(TotalWidth, TotalHeight));
}

UAlpha_HexagonTileDataBase* UAlpha_HexagonTileView::GetTileListItem(int32 inIndex)
{
	if (false == TileDataList.IsValidIndex(inIndex))
	{
		return nullptr;
	}

	return TileDataList[inIndex];
}

void UAlpha_HexagonTileView::ClearHexagonTileList()
{
	for (int32 i = 0; i < HexagonTileList.Num(); i++)
	{
		TilePanel->RemoveChild(HexagonTileList[i].Get());
	}

	TileDataList.Reset();
	HexagonTileList.Reset();
}

void UAlpha_HexagonTileView::OnTouchMove()
{
	if (false == IsValid(TouchButton))
		return;

	FVector2D movePosition = TouchButton->GetTouchMovePosition();
	OnTouchStart(movePosition);
}

void UAlpha_HexagonTileView::OnClicked()
{
	if (false == IsValid(TouchButton))
		return;

	FVector2D startPosition = TouchButton->GetTouchStartPosition();
	OnTouchStart(startPosition);
}

void UAlpha_HexagonTileView::SetSelectTile(int32 inIndex)
{
	if (false == TileDataList.IsValidIndex(inIndex))
		return;

	if (IsValid(SelectTileData) && UseMultipleSelect == false)
	{
		SelectTileData->SetIsSelect(false);
		SelectTileData->Refresh();
	}

	SelectTileData = TileDataList[inIndex];

	if (OnSelectHexagonTile.IsBound())
	{
		OnSelectHexagonTile.Execute(TileDataList[inIndex]->GetTopPostion());
	}

	if (false == IsValid(SelectTileData))
		return;

	SelectTileData->SetIsSelect(true);
	SelectTileData->Refresh();
}

void UAlpha_HexagonTileView::OnTouchStart(const FVector2D inPostion)
{
	//UE_LOG(LogTemp, Warning, TEXT("UAlpha_HexagonTileView::OnTouchStart() - inPostion(%f, %f)"), inPostion.X, inPostion.Y);
	int32 selectRow = 0;
	int32 selectColumn = 0;
	int32 selectIndex = 0;
	float threeQuartersHeight = TileHeight - TileQuarterHeight;

	if (inPostion.Y > TileQuarterHeight)
		selectRow = (inPostion.Y - TileQuarterHeight) / threeQuartersHeight;

	if ((selectRow & 0x0001))
	{

		selectColumn = inPostion.X - TileHalfWidth;
		if (selectColumn <= 0)
		{
			selectColumn = 0;
		}
		else
		{
			selectColumn = selectColumn / TileWidth;
		}
	}
	else
	{
		selectColumn = inPostion.X / TileWidth;
	}


	if (selectColumn >= Column)
		selectColumn = 1 - Column;

	selectIndex = selectRow * Column + selectColumn;

	if (false == TileDataList.IsValidIndex(selectIndex))
		return;

	if (TileDataList[selectIndex]->CollisionCheck(inPostion))
	{
		if (IsValid(SelectTileData) && UseMultipleSelect == false)
		{
			SelectTileData->SetIsSelect(false);
			SelectTileData->Refresh();
		}

		SelectTileData = TileDataList[selectIndex];

		if (OnSelectHexagonTile.IsBound())
		{
			OnSelectHexagonTile.Execute(TileDataList[selectIndex]->GetTopPostion());
		}
	}
	else
	{
		TArray<FVector2D> neighborsCoordinates;
		TileDataList[selectIndex]->GetAroundDoordinatesList(neighborsCoordinates);

		for (int32 i = 0; i < neighborsCoordinates.Num(); i++)
		{
			if (NearestTileCollisionCheck(inPostion, neighborsCoordinates[i]) == true)
				break;
		}
	}

	if (false == IsValid(SelectTileData))
		return;

	SelectTileData->SetIsSelect(true);
	SelectTileData->Refresh();
}

bool UAlpha_HexagonTileView::NearestTileCollisionCheck(FVector2D inPostion, FVector2D inNeighbors)
{
	if (inNeighbors.X >= Column)
		return false;

	if (inNeighbors.X < 0 || inNeighbors.Y < 0)
		return false;


	int32 selectIndex = inNeighbors.Y * Column + inNeighbors.X;
	if (TileDataList.IsValidIndex(selectIndex) == false)
		return false;

	if (TileDataList[selectIndex]->CollisionCheck(inPostion))
	{
		if (IsValid(SelectTileData) && UseMultipleSelect == false)
		{
			SelectTileData->SetIsSelect(false);
			SelectTileData->Refresh();
		}

		TileDataList[selectIndex]->SetIsSelect(true);
		TileDataList[selectIndex]->Refresh();

		SelectTileData = TileDataList[selectIndex];

		if (OnSelectHexagonTile.IsBound())
		{
			OnSelectHexagonTile.Execute(TileDataList[selectIndex]->GetTopPostion());
		}

		return true;
	}

	return false;

}