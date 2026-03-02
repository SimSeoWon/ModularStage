#include "ModularStageEditorView_HexTile.h"
#include "Components/EditableTextBox.h"
#include "Components/Button.h"
#include "Components/ComboBoxString.h"
#include "Components/TextBlock.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Engine/DataTable.h"
#include "UObject/SavePackage.h"
#include "Editor.h"
#include "ModularStage/UI/Hexagon/ModularStage_HexTileView.h"
#include "ModularStage/UI/Hexagon/ModularStage_HexTileDataBase.h"
#include "ModularStage/Table/TableHexGrid.h"

void UModularStageEditorView_HexTile::NativeConstruct()
{
	Super::NativeConstruct();

	if (Btn_CreateTile) Btn_CreateTile->OnClicked.AddDynamic(this, &UModularStageEditorView_HexTile::OnClicked_CreateTile);
	if (Btn_SaveFile) Btn_SaveFile->OnClicked.AddDynamic(this, &UModularStageEditorView_HexTile::OnClicked_SaveFile);
	if (Btn_LoadFile) Btn_LoadFile->OnClicked.AddDynamic(this, &UModularStageEditorView_HexTile::OnClicked_LoadFile);

	if (EditTxt_PrefabPath) EditTxt_PrefabPath->OnTextChanged.AddDynamic(this, &UModularStageEditorView_HexTile::OnChanged_PrefabAssetPath);
	if (EditTxt_ExecutionOrder) EditTxt_ExecutionOrder->OnTextChanged.AddDynamic(this, &UModularStageEditorView_HexTile::OnChanged_ExecutionOrder);
	if (ComboBox_MissionType) ComboBox_MissionType->OnSelectionChanged.AddDynamic(this, &UModularStageEditorView_HexTile::OnChanged_MissionType);

	if (HexagonView)
	{
		HexagonView->OnSelectHexagonTile.BindUObject(this, &UModularStageEditorView_HexTile::OnSelected_HexagonTile);
	}

	// 미션 타입 초기화
	if (ComboBox_MissionType)
	{
		ComboBox_MissionType->ClearOptions();
		ComboBox_MissionType->AddOption(TEXT("Main"));
		ComboBox_MissionType->AddOption(TEXT("Sub"));
	}

	// 현재 월드 이름에 따른 파일명 자동 설정
	if (UWorld* World = GetWorld())
	{
		FString WorldName = World->GetName();
		CurrentFileName = FString::Printf(TEXT("res_%s"), *WorldName);
		CurrentFilePath = FString::Printf(TEXT("/Game/GameData/HexGrid/%s"), *CurrentFileName);
	}
}

void UModularStageEditorView_HexTile::OnClicked_CreateTile()
{
	if (!HexagonView) return;

	float TileSize = FCString::Atof(*TextBox_TileSize->GetText().ToString());
	int32 Column = FCString::Atoi(*TextBox_Column->GetText().ToString());
	int32 Count = FCString::Atoi(*TextBox_Count->GetText().ToString());

	HexagonView->SetTileListItems(TileSize, Column, Count);
}

void UModularStageEditorView_HexTile::OnSelected_HexagonTile(const FVector2D InPosition)
{
	if (HexagonView && HexagonView->SelectTileData)
	{
		UpdateTileInfoUI(HexagonView->SelectTileData);
	}
}

void UModularStageEditorView_HexTile::UpdateTileInfoUI(UModularStage_HexTileDataBase* InTile)
{
	if (!InTile) return;

	Txt_TileIndex->SetText(FText::AsNumber(InTile->Index));
	EditTxt_PrefabPath->SetText(FText::FromString(InTile->PrefabAssetPath));
	EditTxt_ExecutionOrder->SetText(FText::AsNumber(InTile->ExecutionOrder));
	ComboBox_MissionType->SetSelectedOption(InTile->MissionType == EMissionType::Main ? TEXT("Main") : TEXT("Sub"));
}

void UModularStageEditorView_HexTile::OnChanged_PrefabAssetPath(const FText& InText)
{
	if (HexagonView && HexagonView->SelectTileData)
	{
		HexagonView->SelectTileData->PrefabAssetPath = InText.ToString();
	}
}

void UModularStageEditorView_HexTile::OnChanged_ExecutionOrder(const FText& InText)
{
	if (HexagonView && HexagonView->SelectTileData)
	{
		HexagonView->SelectTileData->ExecutionOrder = FCString::Atoi(*InText.ToString());
	}
}

void UModularStageEditorView_HexTile::OnChanged_MissionType(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (HexagonView && HexagonView->SelectTileData)
	{
		HexagonView->SelectTileData->MissionType = (SelectedItem == TEXT("Main")) ? EMissionType::Main : EMissionType::Sub;
	}
}

void UModularStageEditorView_HexTile::OnClicked_SaveFile()
{
	if (!HexagonView || CurrentFilePath.IsEmpty()) return;

	UPackage* Package = CreatePackage(*CurrentFilePath);
	if (!Package) return;

	UDataTable* DataTable = NewObject<UDataTable>(Package, *CurrentFileName, RF_Public | RF_Standalone);
	DataTable->RowStruct = FRES_HEXAGONTILEMAP::StaticStruct();

	FRES_HEXAGONTILEMAP RowData;
	RowData.TileSize = HexagonView->TileSize;
	RowData.Column = HexagonView->Column;
	
	for (auto* TileData : HexagonView->TileDataList)
	{
		FHexagonTile Tile;
		Tile.Index = TileData->Index;
		Tile.TileType = TileData->TileType;
		Tile.HeightType = TileData->HeightType;
		Tile.VecCenter = FVector(TileData->Center.X, TileData->Center.Y, 0.0f); // 2D Center to 3D
		Tile.PrefabAssetPath = TileData->PrefabAssetPath;
		Tile.MissionType = TileData->MissionType;
		Tile.ExecutionOrder = TileData->ExecutionOrder;
		
		RowData.TileList.Add(Tile);
	}

	DataTable->AddRow(FName(TEXT("DEFAULT")), RowData);

	FAssetRegistryModule::AssetCreated(DataTable);
	Package->MarkPackageDirty();

	FString PackageFileName = FPackageName::LongPackageNameToFilename(CurrentFilePath, FPackageName::GetAssetPackageExtension());
	UPackage::SavePackage(Package, DataTable, RF_Public | RF_Standalone, *PackageFileName);
	
	UE_LOG(LogTemp, Log, TEXT("HexGrid saved to %s"), *CurrentFilePath);
}

void UModularStageEditorView_HexTile::OnClicked_LoadFile()
{
	UDataTable* DataTable = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), nullptr, *CurrentFilePath));
	if (!DataTable) return;

	FRES_HEXAGONTILEMAP* RowData = DataTable->FindRow<FRES_HEXAGONTILEMAP>(FName(TEXT("DEFAULT")), TEXT(""));
	if (RowData)
	{
		TextBox_TileSize->SetText(FText::AsNumber(RowData->TileSize));
		TextBox_Column->SetText(FText::AsNumber(RowData->Column));
		TextBox_Count->SetText(FText::AsNumber(RowData->TileList.Num()));

		TArray<UModularStage_HexTileDataBase*> NewTileList;
		for (const auto& Tile : RowData->TileList)
		{
			UModularStage_HexTileDataBase* NewData = NewObject<UModularStage_HexTileDataBase>(this);
			NewData->Index = Tile.Index;
			NewData->TileType = Tile.TileType;
			NewData->HeightType = Tile.HeightType;
			NewData->PrefabAssetPath = Tile.PrefabAssetPath;
			NewData->MissionType = Tile.MissionType;
			NewData->ExecutionOrder = Tile.ExecutionOrder;
			
			NewTileList.Add(NewData);
		}

		HexagonView->TileSize = RowData->TileSize;
		HexagonView->Column = RowData->Column;
		HexagonView->SetTileListItems(NewTileList);
	}
}
