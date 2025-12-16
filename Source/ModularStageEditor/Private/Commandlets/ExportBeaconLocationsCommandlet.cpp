#include "Commandlets/ExportBeaconLocationsCommandlet.h"
#include "ModularStage/Mission/Beacon.h"
#include "HAL/FileManager.h"
#include "Misc/FileHelper.h"
#include "Misc/Paths.h"
#include "Engine/ObjectLibrary.h"
#include "Engine/World.h"
#include "Editor.h"
#include "EngineUtils.h"
#include "Modules/ModuleManager.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "FileHelpers.h"
#include "Editor/EditorEngine.h" // For GEditor

UExportBeaconLocationsCommandlet::UExportBeaconLocationsCommandlet()
{
	IsClient = false;
	IsServer = false;
	IsEditor = true;
	LogToConsole = true;
}

int32 UExportBeaconLocationsCommandlet::Main(const FString& Params)
{
	UE_LOG(LogTemp, Display, TEXT("Starting UExportBeaconLocationsCommandlet"));

	const FString LevelsPath = TEXT("/Game/maps/_LevelToExport/");
	UE_LOG(LogTemp, Display, TEXT("Searching for levels in path: %s"), *LevelsPath);

	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));
	TArray<FAssetData> AssetDatas;
	AssetRegistryModule.Get().GetAssetsByPath(FName(*LevelsPath), AssetDatas, true);

	if (AssetDatas.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No levels found in path: %s"), *LevelsPath);
		return 1;
	}

	FString CSVContent = TEXT("Level,ActorName,X,Y,Z\n");
    
	for (const FAssetData& AssetData : AssetDatas)
	{
		if (AssetData.GetClass() != UWorld::StaticClass())
			continue;

		const FString LevelPath = AssetData.GetObjectPathString();
		UE_LOG(LogTemp, Display, TEXT("Processing level: %s"), *LevelPath);
		
		// FEditorFileUtils::LoadMap returns bool, not UWorld*
		bool bMapLoadedSuccessfully = FEditorFileUtils::LoadMap(*LevelPath, false, true); // Path, bLoadAsTemplate, bShowProgress

		UWorld* World = nullptr;
		if (bMapLoadedSuccessfully && GEditor)
		{
			World = GEditor->GetEditorWorldContext().World();
		}

		if (!World || !World->PersistentLevel)
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load map or find persistent level: %s"), *LevelPath);
			CollectGarbage(RF_NoFlags);
			continue;
		}

		const FString LevelName = FPaths::GetBaseFilename(LevelPath);
		for (AActor* Actor : World->PersistentLevel->Actors)
		{
			if (ABeacon* Beacon = Cast<ABeacon>(Actor))
			{
				const FVector Location = Beacon->GetActorLocation();
				const FString ActorName = Beacon->GetName();
				CSVContent += FString::Printf(TEXT("%s,%s,%.3f,%.3f,%.3f\n"), *LevelName, *ActorName, Location.X, Location.Y, Location.Z);
			}
		}

		CollectGarbage(RF_NoFlags);
	}

	FString FilePath = FPaths::ProjectSavedDir() / TEXT("BeaconLocations.csv");
	if (FFileHelper::SaveStringToFile(CSVContent, *FilePath))
	{
		UE_LOG(LogTemp, Display, TEXT("Successfully exported beacon locations to: %s"), *FilePath);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to save CSV file to: %s"), *FilePath);
		return 1;
	}

	UE_LOG(LogTemp, Display, TEXT("Finished UExportBeaconLocationsCommandlet"));
	return 0;
}
