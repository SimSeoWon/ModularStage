// Copyright Epic Games, Inc. All Rights Reserved.

#include "ModularStageEditor.h"
#include "Modules/ModuleManager.h"
#include "Widgets/Docking/SDockTab.h"

#include "EditorMenuHelper.h"
#include "Support/EditorSupport.h"
#include "Widget/StagePresetEditor.h"



#define LOCTEXT_NAMESPACE "ModularStageEditor"
//IMPLEMENT_PRIMARY_GAME_MODULE( FDefaultGameModuleImpl, ModularStageEditor, "ModularStageEditor" );

FName FModularStageEditor::TabName = FName("ModularStageEditorTab");

void FModularStageEditor::StartupModule()
{
	MenuHelper = MakeShareable(new FEditorMenuHelper);
	if (MenuHelper.IsValid())
	{
		MenuHelper->InitMenu();
		MenuHelper->AddSheerToolMenuEntry(TEXT("ModularStage"), TEXT("Modular Stage Editor"), FText::FromString("Modular Stage Editor"), FText::FromString("Modular Stage Editor"), FSlateIcon(), FToolUIActionChoice(FExecuteAction::CreateRaw(this, &FModularStageEditor::OnClicked_Menu)));
	}

	FWorldDelegates::OnWorldCleanup.AddRaw(this, &FModularStageEditor::OnWorldCleanup);	
}

void FModularStageEditor::ShutdownModule()
{
	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(TabName);
}

TSharedRef<SDockTab> FModularStageEditor::CreateTabWidget(const FSpawnTabArgs& Args)
{
	/**
	
	*/
	return SNew(SDockTab).TabRole(ETabRole::NomadTab);
}

void FModularStageEditor::OnWorldCleanup(UWorld* World, bool bSessionEnded, bool bCleanupResources)
{
	TSharedPtr<SDockTab> tab = FGlobalTabmanager::Get()->FindExistingLiveTab(TabName);
	if (tab.IsValid()) 
	{
		tab.Get()->RequestCloseTab();
	}
}

void FModularStageEditor::OnClicked_Menu()
{
	FName tabID;
	const FString DeleteInactiveEmitterUIPath = TEXT("EditorUtilityWidgetBlueprint'/Game/nevercook/editor/BP_StagePresetEditor.BP_StagePresetEditor'");
	UStagePresetEditor* editorWidget = FEditorSupport::ShowEditorUtilityWidget<UStagePresetEditor>(DeleteInactiveEmitterUIPath, tabID);
	if (IsValid(editorWidget))
	{
		//editorWidget->Run();
	}
}


#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FModularStageEditor, ModularStageEditor)
