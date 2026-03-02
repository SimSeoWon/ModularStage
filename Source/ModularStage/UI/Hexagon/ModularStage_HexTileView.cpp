#include "ModularStage_HexTileView.h"
#include "ModularStage_HexTileDataBase.h"
#include "ModularStage_HexTouchButton.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"

void UModularStage_HexTileView::NativeConstruct()
{
	Super::NativeConstruct();

	if (TouchButton)
	{
		TouchButton->OnClicked.AddDynamic(this, &UModularStage_HexTileView::OnClicked);
		TouchButton->OnPressed.AddDynamic(this, &UModularStage_HexTileView::OnClicked);
		TouchButton->OnTouchMove.AddDynamic(this, &UModularStage_HexTileView::OnTouchMove);
	}
}

void UModularStage_HexTileView::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	// 위젯 내부의 슬레이트 자원을 해제하여 메모리 유출 방지
	if (TouchButton) TouchButton->ReleaseSlateResources(bReleaseChildren);
	if (RootPanel) RootPanel->ReleaseSlateResources(bReleaseChildren);
	if (TilePanel) TilePanel->ReleaseSlateResources(bReleaseChildren);

	// 리스트에 저장된 위젯들도 초기화
	HexagonTileList.Reset();
}

void UModularStage_HexTileView::OnClicked()
{
	if (TouchButton)
	{
		OnTouchStart(TouchButton->GetTouchStartPosition());
	}
}

void UModularStage_HexTileView::OnTouchMove()
{
	if (TouchButton)
	{
		OnTouchStart(TouchButton->GetTouchMovePosition());
	}
}

void UModularStage_HexTileView::SynchronizeProperties()
{
	Super::SynchronizeProperties();

#if WITH_EDITORONLY_DATA
	if (IsDesignTime() && bEdit_Preview && Edit_Count > 0)
	{
		TArray<UModularStage_HexTileDataBase*> ArrTiles;
		for (int32 i = 0; i < Edit_Count; i++)
		{
			UModularStage_HexTileDataBase* NewData = NewObject<UModularStage_HexTileDataBase>(this);
			ArrTiles.Add(NewData);
		}
		SetTileListItems(ArrTiles);
	}
#endif
}

void UModularStage_HexTileView::SetTileListItems(float InTileSize, int32 InColumn, int32 InCount, float InScale)
{
	TileSize = InTileSize;
	Scale = InScale;
	Column = InColumn;

	TArray<UModularStage_HexTileDataBase*> ArrTiles;
	float CorrectedSize = TileSize * Scale;
	float Width = FMath::Sqrt(3.0f) * CorrectedSize;
	float Height = 2 * CorrectedSize;

	for (int32 i = 0; i < InCount; i++)
	{
		UModularStage_HexTileDataBase* NewData = NewObject<UModularStage_HexTileDataBase>(this);
		NewData->Index = i;
		FVector2D Coords(i % Column, i / Column);
		NewData->Init(Coords, CorrectedSize, Width, Height, Scale);
		ArrTiles.Add(NewData);
	}

	SetTileListItems(ArrTiles);
}

void UModularStage_HexTileView::SetTileListItems(const TArray<UModularStage_HexTileDataBase*>& InListItems)
{
	ClearHexagonTileList();
	TileDataList.Append(InListItems);

	float CorrectedSize = TileSize * Scale;
	TileHeight = 2 * CorrectedSize;
	TileWidth = FMath::Sqrt(3.0f) * CorrectedSize;
	TileHalfHeight = CorrectedSize;
	TileQuarterHeight = TileHalfHeight / 2;
	TileHalfWidth = TileWidth / 2;

	for (int32 i = 0; i < TileDataList.Num(); i++)
	{
		UModularStage_HexTileDataBase* TileData = TileDataList[i];
		if (!TileData) continue;

		FVector2D Coords(i % Column, i / Column);
		TileData->Init(Coords, CorrectedSize, TileWidth, TileHeight, Scale);

		if (UClass* WidgetClass = HexagonTileAsset.LoadSynchronous())
		{
			UModularStage_HexTileItem* TileWidget = CreateWidget<UModularStage_HexTileItem>(GetWorld(), WidgetClass);
			if (TileWidget)
			{
				TilePanel->AddChild(TileWidget);
				TileWidget->SetRenderTranslation(TileData->Center);
				
				if (UCanvasPanelSlot* CPSSlot = Cast<UCanvasPanelSlot>(TileWidget->Slot))
				{
					CPSSlot->SetOffsets(FMargin(0, 0, 0, 0));
					CPSSlot->SetAutoSize(true);
				}

				TileWidget->NativeOnListItemObjectSet(TileData);
				TileData->SetTileObject(TileWidget);
				HexagonTileList.Emplace(TileWidget);
			}
		}
	}

	// 전체 크기 계산
	int32 TileCount = TileDataList.Num();
	if (Column >= TileCount)
	{
		TotalWidth = TileCount * TileWidth;
		TotalHeight = TileHeight;
	}
	else
	{
		int32 RowCount = (TileCount + Column - 1) / Column;
		TotalWidth = Column * TileWidth + TileHalfWidth;
		TotalHeight = (TileHeight - TileQuarterHeight) * RowCount + TileQuarterHeight;
	}

	if (UCanvasPanelSlot* RootSlot = Cast<UCanvasPanelSlot>(RootPanel->Slot))
	{
		RootSlot->SetSize(FVector2D(TotalWidth, TotalHeight));
	}
}

void UModularStage_HexTileView::SetScale(float InScale)
{
	Scale = InScale;
	float CorrectedSize = TileSize * Scale;
	TileHeight = 2 * CorrectedSize;
	TileWidth = FMath::Sqrt(3.0f) * CorrectedSize;
	TileHalfHeight = CorrectedSize;
	TileQuarterHeight = TileHalfHeight / 2;
	TileHalfWidth = TileWidth / 2;

	for (int32 i = 0; i < TileDataList.Num(); i++)
	{
		UModularStage_HexTileDataBase* TileData = TileDataList[i];
		if (!TileData) continue;

		FVector2D Coords(i % Column, i / Column);
		TileData->Init(Coords, CorrectedSize, TileWidth, TileHeight, Scale);

		if (HexagonTileList.IsValidIndex(i) && HexagonTileList[i].IsValid())
		{
			HexagonTileList[i]->SetRenderTranslation(TileData->Center);
			TileData->Refresh();
		}
	}

	// 전체 크기 재계산
	int32 TileCount = TileDataList.Num();
	if (Column >= TileCount)
	{
		TotalWidth = TileCount * TileWidth;
		TotalHeight = TileHeight;
	}
	else
	{
		int32 RowCount = (TileCount + Column - 1) / Column;
		TotalWidth = Column * TileWidth + TileHalfWidth;
		TotalHeight = (TileHeight - TileQuarterHeight) * RowCount + TileQuarterHeight;
	}

	if (UCanvasPanelSlot* RootSlot = Cast<UCanvasPanelSlot>(RootPanel->Slot))
	{
		RootSlot->SetSize(FVector2D(TotalWidth, TotalHeight));
	}
}

UModularStage_HexTileDataBase* UModularStage_HexTileView::GetTileListItem(int32 InIndex)
{
	return TileDataList.IsValidIndex(InIndex) ? TileDataList[InIndex] : nullptr;
}

void UModularStage_HexTileView::ClearHexagonTileList()
{
	if (TilePanel) TilePanel->ClearChildren();
	TileDataList.Reset();
	HexagonTileList.Reset();
}

void UModularStage_HexTileView::SetSelectTile(int32 InIndex)
{
	if (!TileDataList.IsValidIndex(InIndex)) return;

	if (SelectTileData && !bUseMultipleSelect)
	{
		SelectTileData->SetIsSelect(false);
		SelectTileData->Refresh();
	}

	SelectTileData = TileDataList[InIndex];
	SelectTileData->SetIsSelect(true);
	SelectTileData->Refresh();

	if (OnSelectHexagonTile.IsBound())
	{
		OnSelectHexagonTile.Execute(SelectTileData->GetTopPostion());
	}
}

void UModularStage_HexTileView::OnTouchStart(const FVector2D InPosition)
{
	float ThreeQuartersHeight = TileHeight - TileQuarterHeight;
	int32 SelectRow = (InPosition.Y > TileQuarterHeight) ? (InPosition.Y - TileQuarterHeight) / ThreeQuartersHeight : 0;
	int32 SelectCol = 0;

	if (SelectRow & 1)
	{
		float OffsetX = InPosition.X - TileHalfWidth;
		SelectCol = (OffsetX > 0) ? OffsetX / TileWidth : 0;
	}
	else
	{
		SelectCol = InPosition.X / TileWidth;
	}

	if (SelectCol >= Column) SelectCol = Column - 1;

	int32 Index = SelectRow * Column + SelectCol;
	if (TileDataList.IsValidIndex(Index))
	{
		if (TileDataList[Index]->CollisionCheck(InPosition))
		{
			SetSelectTile(Index);
		}
		else
		{
			TArray<FVector2D> Neighbors;
			TileDataList[Index]->GetAroundDoordinatesList(Neighbors);
			for (const FVector2D& NeighborCoords : Neighbors)
			{
				if (NearestTileCollisionCheck(InPosition, NeighborCoords)) break;
			}
		}
	}
}

bool UModularStage_HexTileView::NearestTileCollisionCheck(FVector2D InPosition, FVector2D InNeighbors)
{
	if (InNeighbors.X < 0 || InNeighbors.X >= Column || InNeighbors.Y < 0) return false;

	int32 Index = InNeighbors.Y * Column + InNeighbors.X;
	if (TileDataList.IsValidIndex(Index) && TileDataList[Index]->CollisionCheck(InPosition))
	{
		SetSelectTile(Index);
		return true;
	}
	return false;
}
