#pragma once

#include "CoreMinimal.h"
#include "WorkflowOrientedApp/WorkflowTabFactory.h"

struct FWorkflowTabFactory_Mission : public FWorkflowTabFactory
{
public:
	FWorkflowTabFactory_Mission(TSharedPtr<class FAssetEditorToolkit> InHostingToolkit);

	virtual TSharedRef<SWidget> CreateTabBody(const FWorkflowTabSpawnInfo& Info) const override;
	virtual FText GetTabToolTipText(const FWorkflowTabSpawnInfo& Info) const override;

	static const FName TabID;
};
