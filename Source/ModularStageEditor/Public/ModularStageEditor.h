// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "Modules/ModuleInterface.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Toolkits/SimpleAssetEditor.h"
#include "Toolkits/IToolkit.h"

class FToolBarBuilder;
class FMenuBarBuilder;
class FEditorMenuHelper;
class FEditorContextMenu;
class FEditorToolBarExtension;

class FModularStageEditor : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;


	UFUNCTION()
	void OnClicked_Menu();

	UFUNCTION()
	TSharedRef<SDockTab> CreateTabWidget(const FSpawnTabArgs& Args);

	UFUNCTION()
	void OnWorldCleanup(UWorld* World, bool bSessionEnded, bool bCleanupResources);

public:
	static FName TabName;

protected:
	
	TSharedPtr<FEditorMenuHelper> MenuHelper;
};