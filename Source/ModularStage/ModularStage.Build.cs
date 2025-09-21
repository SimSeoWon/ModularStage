// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ModularStage : ModuleRules
{
	public ModularStage(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		PrivatePCHHeaderFile = "ModularStagePCH.h";

        PublicDependencyModuleNames.AddRange(new string[] { 
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"NavigationSystem", 
			"AIModule", 
			"Niagara", 
			"EnhancedInput",
            "UnrealEd",
		});

		if (Target.bBuildEditor == true)
		{
            PublicDependencyModuleNames.AddRange(new string[] {
            "AssetRegistry",
            "UnrealEd",
            "BlueprintEditorLibrary",});

            PrivateDependencyModuleNames.AddRange(new string[] { "UnrealEd", "Kismet" });
		}
    }
}
