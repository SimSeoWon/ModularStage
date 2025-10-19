// Copyright Epic Games, Inc. All Rights Reserved.

using System.Collections.Generic;
using System.IO;
using UnrealBuildTool;

public class ModularStageEditor : ModuleRules
{
	public ModularStageEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		//PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
        PrivatePCHHeaderFile = "../ModularStage/ModularStagePCH.h";
        bLegacyPublicIncludePaths = false;
        ShadowVariableWarningLevel = WarningLevel.Error;
        DefaultBuildSettings = BuildSettingsVersion.V5;

        OptimizeCode = CodeOptimization.Never;
        //bUseUnity = true;

        PublicDependencyModuleNames.AddRange(new string[]
            {
                "ModularStage",
                "UnrealEd",
                "EditorScriptingUtilities",
                "HTTP",
                "Json",
                "JsonUtilities",
                "Blutility",
                "AnimationModifiers",
                "UMG",
                "UnrealEd",
                "ToolMenus",
                "AssetTools",
                "Synthesis",
                "NiagaraEditor",
                "SourceControl",
                "AnimationSharing",
                "NiagaraAnimNotifies",
                "SkeletalMeshUtilitiesCommon",
                "ContentBrowser",
                "ContentBrowserData",
                "ScriptableEditorWidgets",
                "NavigationSystem",
                "StaticMeshDescription",
                "MeshDescription",
                "AssetManagerEditor",
                "BlueprintEditorLibrary",
            });

		PrivateDependencyModuleNames.AddRange(new string[] 
            {
                "Core",
                "EditorFramework",
                "CoreUObject",
                "Engine",
                "InputCore",
                "RenderCore",
                "AssetRegistry",
                "AssetTools",
                "UnrealEd",
                "EditorStyle",
                "SlateCore",
                "Slate",
                "Landscape",
                "Navmesh",
                "LevelEditor",
                "ProceduralMeshComponent",
                "Projects",
                "AIModule",
                "GameplayTasks",
                "NavigationSystem",
                "UMG",
                "RHI",
                "UMGEditor",
                "Sequencer",
                "ApplicationCore",
                "DesktopPlatform",
                "MainFrame",
                "WorldBrowser",
                "MaterialEditor",
                "Niagara",
                "MovieScene",
                "MovieSceneTracks",
                "LevelSequence",
                "PropertyEditor",
                "DerivedDataCache",
                "Synthesis",
                "DeveloperSettings",
                "Persona",
                "ContentBrowser", });

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true

        AddIncludePath(ref PrivateIncludePaths, "ModularStage");
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
