// Copyright Epic Games, Inc. All Rights Reserved.

#include "ModularStageEditor.h"
#include "Modules/ModuleManager.h"
#include "Widgets/Docking/SDockTab.h"

#include "EditorMenuHelper.h"
#include "Support/EditorSupport.h"
#include "Widget/StagePresetEditor.h"
#include "Widget/EUW_InGameTask.h"


#define LOCTEXT_NAMESPACE "ModularStageEditor"

FName FModularStageEditor::TabName = FName("ModularStageEditorTab");

template <class T, class = typename TEnableIf<TPointerIsConvertibleFromTo<T, UEditorWidgetBase>::Value>::Type>
void RegisterEditorMenu(TSharedPtr<FEditorMenuHelper> MenuHelper, FName SectionName)
{
	if (false == MenuHelper.IsValid())
		return;

	MenuHelper->AddSheerToolMenuEntry(
		SectionName,
		T::GetMenuName(), 
		T::GetMenuDisplayName(),
		T::GetMenuTooltip(),
		FSlateIcon(), // Icon
		FToolUIActionChoice(
			FExecuteAction::CreateLambda([]()
				{
					T* widget = T::template ShowEditorUtilityWidget<T>();
					if (false == IsValid(widget))
						return;

					widget->Run();
				})
		)
	);
}

void FModularStageEditor::StartupModule()
{
	SetEditorMenu();

	FWorldDelegates::OnWorldCleanup.AddRaw(this, &FModularStageEditor::OnWorldCleanup);
}

void FModularStageEditor::ShutdownModule()
{
	UToolMenus::UnRegisterStartupCallback(this);
	UToolMenus::UnregisterOwner(this);

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(TabName);
}

void FModularStageEditor::SetEditorMenu() 
{
	MenuHelper = MakeShareable(new FEditorMenuHelper);
	if (MenuHelper.IsValid())
	{
		MenuHelper->InitMenu();

		// Register the editor menus using the new template function
		RegisterEditorMenu<UStagePresetEditor>(MenuHelper, TEXT("ModularStage"));
		RegisterEditorMenu<UEUW_InGameTask>(MenuHelper, TEXT("MissionTask"));
	}	
}

TSharedRef<SDockTab> FModularStageEditor::CreateTabWidget(const FSpawnTabArgs& Args)
{
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

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FModularStageEditor, ModularStageEditor)
