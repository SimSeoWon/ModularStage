// Copyright Epic Games, Inc. All Rights Reserved.

#include "ModularStageEditor.h"
#include "Modules/ModuleManager.h"
#include "Widgets/Docking/SDockTab.h"
#include "BlueprintEditorModule.h"
#include "BlueprintEditor.h"
#include "WorkflowOrientedApp/WorkflowTabManager.h"
#include "EditorMenuHelper.h"
#include "Support/EditorSupport.h"
#include "Widget/StagePresetEditor.h"
#include "Widget/EUW_InGameTask.h"
#include "Workflow/WorkflowTabFactory_Mission.h"
#include "UnrealEdGlobals.h"
#include "Editor/UnrealEdEngine.h"
#include "Visualizer/FVisualizer_Spawner.h"
#include "Mission/Component/MissionPropsComponent_Spawner.h"


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
	OnRegistration_WorkflowTab();

	FWorldDelegates::OnWorldCleanup.AddRaw(this, &FModularStageEditor::OnWorldCleanup);

	if (GUnrealEd)
	{
		TSharedRef<FComponentVisualizer> Visualizer = MakeShared<FVisualizer_Spawner>();
		GUnrealEd->RegisterComponentVisualizer(UMissionPropsComponent_Spawner::StaticClass()->GetFName(), Visualizer);
		Visualizer->OnRegister();
	}
}

void FModularStageEditor::ShutdownModule()
{
	UToolMenus::UnRegisterStartupCallback(this);
	UToolMenus::UnregisterOwner(this);

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(TabName);

	if (GUnrealEd)
	{
		GUnrealEd->UnregisterComponentVisualizer(UMissionPropsComponent_Spawner::StaticClass()->GetFName());
	}
}

void FModularStageEditor::OnRegistration_WorkflowTab()
{
	FBlueprintEditorModule& module = FModuleManager::LoadModuleChecked<FBlueprintEditorModule>("Kismet");
	RegisterTabsDelegateHandle = module.OnRegisterTabsForEditor().AddLambda(
		// InBlueprintEditor: ���� �����Ǵ� ������rint �������� �ν��Ͻ�
		[](FWorkflowAllowedTabSet& TabFactories, FName InModeName, TSharedPtr<FBlueprintEditor> InBlueprintEditor)
		{
			// FWorkflowTabFactory�� ��ӹ��� �츮 Ŀ���� ���丮�� �����Ͽ� ����մϴ�.
			TSharedPtr<FWorkflowTabFactory_Mission> factory = MakeShareable(new FWorkflowTabFactory_Mission(InBlueprintEditor));
			if (false == factory.IsValid())
				return;

			TabFactories.RegisterFactory(factory);
		}
	);
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
