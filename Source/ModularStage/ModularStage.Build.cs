// Copyright Epic Games, Inc. All Rights Reserved.

using System.Collections.Generic;
using System.IO;
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
            "UMG",
        });

        AddIncludePath(ref PrivateIncludePaths, "ModularStage");

        if (Target.bBuildEditor == true)
		{
            PublicDependencyModuleNames.AddRange(new string[] {
            "AssetRegistry",
            "UnrealEd",
            "BlueprintEditorLibrary",
            "ModularStageEditor",
            "UMGEditor",
            "Blutility",
            });

            PrivateDependencyModuleNames.AddRange(new string[] { "UnrealEd", "Kismet" });

            AddIncludePath(ref PrivateIncludePaths, "ModularStageEditor");
        }

        void AddIncludePath(ref List<string> InPathList, string InRootDir)
        {
            InPathList.Add(InRootDir);

            string sourceDir = ModuleDirectory + "/../"; // sourceDir = "ModularStage/Source/"
            string rootDirFullPath = sourceDir + InRootDir; // sourceDir = "ModularStage/Source/ModularStage"

            AddIncludePathRecursive(ref InPathList, rootDirFullPath, sourceDir.Length);
        }

        void AddIncludePathRecursive(ref List<string> InPathList, string InRootDir, int InSplitLength)
        {
            foreach (string Dir in Directory.GetDirectories(InRootDir))
            {
                string RegDir = Dir.Substring(InSplitLength).Replace('\\', '/');
                if (false == InPathList.Contains(RegDir))
                {
                    InPathList.Add(RegDir);
                }
                AddIncludePathRecursive(ref InPathList, Dir, InSplitLength);
            }
        }
    }
}
