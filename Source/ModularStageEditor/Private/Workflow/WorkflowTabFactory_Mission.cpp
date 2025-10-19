#include "WorkflowTabFactory_Mission.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/STextBlock.h"

const FName FWorkflowTabFactory_Mission::TabID(TEXT("MissionEditor_Mission"));

FWorkflowTabFactory_Mission::FWorkflowTabFactory_Mission(TSharedPtr<class FAssetEditorToolkit> InHostingToolkit)
	: FWorkflowTabFactory(FWorkflowTabFactory_Mission::TabID, InHostingToolkit)
{
	TabLabel = FText::FromString(TEXT("Mission"));
	bIsSingleton = true;
}

TSharedRef<SWidget> FWorkflowTabFactory_Mission::CreateTabBody(const FWorkflowTabSpawnInfo& Info) const
{
	return SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			SNew(STextBlock)
			.Text(FText::FromString(TEXT("Mission Editor Contents")))
		];
}

FText FWorkflowTabFactory_Mission::GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const
{
	return FText::FromString(TEXT("The tab for editing missions."));
}
