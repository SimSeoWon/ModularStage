#pragma once

#include "CoreMinimal.h"
#include "Widget/EditorWidgetBase.h"
#include "ModularStageEditorView_HexTile.generated.h"

class UEditableTextBox;
class UButton;
class UTextBlock;
class UComboBoxString;
class UModularStage_HexTileView;
class UModularStage_HexTileDataBase;

/**
 * ModularStage 육각형 타일맵 에디터 유틸리티 위젯
 */
UCLASS()
class MODULARSTAGEEDITOR_API UModularStageEditorView_HexTile : public UEditorWidgetBase
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	// --- 메뉴 등록 정보 ---
	static FName GetMenuName() { return FName(TEXT("HexGridEditor")); }
	static FText GetMenuDisplayName() { return FText::FromString(TEXT("Hexagon Tile Editor")); }
	static FText GetMenuTooltip() { return FText::FromString(TEXT("Open the Hexagon Tile Map Editor")); }
	static FString GetWidgetBlueprintPath() { return TEXT("/Game/nevercook/editor/menu/BP_HexTileEditor.BP_HexTileEditor"); }

	// --- 에디터 기능 ---
	UFUNCTION() void OnClicked_CreateTile();
	UFUNCTION() void OnClicked_SaveFile();
	UFUNCTION() void OnClicked_LoadFile();
	UFUNCTION() void OnSelected_HexagonTile(const FVector2D InPosition);

	// --- 미션 데이터 편집 ---
	UFUNCTION() void OnChanged_PrefabAssetPath(const FText& InText);
	UFUNCTION() void OnChanged_ExecutionOrder(const FText& InText);
	UFUNCTION() void OnChanged_MissionType(FString SelectedItem, ESelectInfo::Type SelectionType);

private:
	void UpdateTileInfoUI(UModularStage_HexTileDataBase* InTile);

protected:
	// --- 그리드 설정 UI ---
	UPROPERTY(meta = (BindWidget)) 
	UEditableTextBox* TextBox_TileSize = nullptr;
	UPROPERTY(meta = (BindWidget)) 
	UEditableTextBox* TextBox_Column = nullptr;
	UPROPERTY(meta = (BindWidget)) 
	UEditableTextBox* TextBox_Count = nullptr;
	UPROPERTY(meta = (BindWidget)) 
	UButton* Btn_CreateTile = nullptr;

	// --- 미션/프리팹 편집 UI ---
	UPROPERTY(meta = (BindWidget)) 
	UEditableTextBox* EditTxt_PrefabPath = nullptr;
	UPROPERTY(meta = (BindWidget)) 
	UEditableTextBox* EditTxt_ExecutionOrder = nullptr;
	UPROPERTY(meta = (BindWidget)) 
	UComboBoxString* ComboBox_MissionType = nullptr;
	UPROPERTY(meta = (BindWidget)) 
	UTextBlock* Txt_TileIndex = nullptr;

	// --- 타일맵 뷰 ---
	UPROPERTY(meta = (BindWidget)) 
	UModularStage_HexTileView* HexagonView = nullptr;

	// --- 저장 관련 ---
	UPROPERTY(meta = (BindWidget)) 
	UButton* Btn_SaveFile = nullptr;
	UPROPERTY(meta = (BindWidget)) 
	UButton* Btn_LoadFile = nullptr;

	FString CurrentFilePath;
	FString CurrentFileName;
};
