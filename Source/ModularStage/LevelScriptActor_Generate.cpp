// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelScriptActor_Generate.h"
#include "EngineUtils.h"
#include "Engine/Blueprint.h"
#include "BlueprintEditorLibrary.h"
#include "MissionPrefab.h"
#include "UObject/SavePackage.h"
#if WITH_EDITOR
#include "Kismet2/KismetEditorUtilities.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Misc/PackageName.h"
#include "UObject/Package.h"
#include "Editor.h"
#include "UnrealEd.h"
#include "AssetToolsModule.h"
#include "Factories/BlueprintFactory.h"
#include "Engine/SimpleConstructionScript.h"
#include "Kismet2/BlueprintEditorUtils.h"
#endif

// Sets default values
ALevelScriptActor_Generate::ALevelScriptActor_Generate()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ALevelScriptActor_Generate::BeginPlay()
{
	Super::BeginPlay();
}

void ALevelScriptActor_Generate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

TArray<AActor*> ALevelScriptActor_Generate::GetActorsInFolder(const FString& inFolderPath)
{
	TArray<AActor*> ActorsInFolder;
	UWorld* World = GetWorld();
	if (!World) return ActorsInFolder;

#if WITH_EDITOR
	for (TActorIterator<AActor> It(World); It; ++It)
	{
		AActor* Actor = *It;
		if (Actor)
		{
			const FName PathName = Actor->GetFolderPath();
			if (!PathName.IsNone() && PathName.ToString() == inFolderPath)
			{
				ActorsInFolder.Add(Actor);
			}
		}
	}
#endif
	return ActorsInFolder;
}

void ALevelScriptActor_Generate::CreatePrefabFromActors(const TArray<AActor*>& inActors, const FString& inSavePath)
{
#if WITH_EDITOR
	if (inActors.Num() <= 0) return;

	FString BasePackageName = inSavePath;
	FString UniquePackageName = BasePackageName;
	int32 Suffix = 1;
	while (FPackageName::DoesPackageExist(UniquePackageName))
	{
		UniquePackageName = FString::Printf(TEXT("%s_%d"), *BasePackageName, Suffix++);
	}

	UBlueprint* TempBlueprint = FKismetEditorUtilities::CreateBlueprintFromActors(UniquePackageName, inActors);
	if (!IsValid(TempBlueprint)) return;

	UClass* NewParentClass = AMissionPrefab::StaticClass();
	UBlueprintEditorLibrary::ReparentBlueprint(TempBlueprint, NewParentClass);
	FKismetEditorUtilities::CompileBlueprint(TempBlueprint);

	TempBlueprint->MarkPackageDirty();
	FAssetRegistryModule::AssetCreated(TempBlueprint);

	const FString PackageName = TempBlueprint->GetPackage()->GetName();
	const FString PackageFileName = FPackageName::LongPackageNameToFilename(PackageName, FPackageName::GetAssetPackageExtension());

	FSavePackageArgs SaveArgs;
	SaveArgs.TopLevelFlags = RF_Standalone;
	SaveArgs.SaveFlags = SAVE_NoError;
	UPackage::SavePackage(TempBlueprint->GetPackage(), nullptr, *PackageFileName, SaveArgs);
#endif
}

void ALevelScriptActor_Generate::CreatePrefabFromCollectedComponents(const TArray<AActor*>& InActors, const FString& inSavePath)
{
#if WITH_EDITOR
	if (InActors.Num() <= 0) return;

	FString BasePackageName = inSavePath;
	FString UniquePackageName = BasePackageName;
	int32 Suffix = 1;
	while (FPackageName::DoesPackageExist(UniquePackageName))
	{
		UniquePackageName = FString::Printf(TEXT("%s_%d"), *BasePackageName, Suffix++);
	}

	FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools");
	UBlueprintFactory* BlueprintFactory = NewObject<UBlueprintFactory>();
	BlueprintFactory->ParentClass = AMissionPrefab::StaticClass();

	UObject* NewAsset = AssetToolsModule.Get().CreateAsset(FPackageName::GetLongPackageAssetName(UniquePackageName), FPackageName::GetLongPackagePath(UniquePackageName), UBlueprint::StaticClass(), BlueprintFactory);

	UBlueprint* NewBlueprint = Cast<UBlueprint>(NewAsset);
	if (!NewBlueprint) return;

	USimpleConstructionScript* SCS = NewBlueprint->SimpleConstructionScript;
	if (!SCS) return;

	TMap<UActorComponent*, USCS_Node*> OriginalToNewNodeMap;

	for (AActor* OriginalActor : InActors)
	{
		if (!OriginalActor) continue;
		TArray<UActorComponent*> Components;
		OriginalActor->GetComponents(Components);
		for (UActorComponent* Comp : Components)
		{
			// 에디터 전용 컴포넌트는 복사하지 않습니다.
			if (Comp && !Comp->IsEditorOnly())
			{
				USCS_Node* NewNode = SCS->CreateNode(Comp->GetClass(), Comp->GetFName());
				SCS->AddNode(NewNode);
				OriginalToNewNodeMap.Add(Comp, NewNode);

				if (NewNode->ComponentTemplate)
				{
					UEngine::CopyPropertiesForUnrelatedObjects(Comp, NewNode->ComponentTemplate);
				}
			}
		}
	}

	for (auto const& Elem : OriginalToNewNodeMap)
	{
		UActorComponent* OriginalComp = Elem.Key;
		USCS_Node* NewNode = Elem.Value;

		USceneComponent* OriginalSceneComp = Cast<USceneComponent>(OriginalComp);
		if (!OriginalSceneComp) continue;

		USceneComponent* OriginalParentComp = OriginalSceneComp->GetAttachParent();
		USCS_Node* NewParentNode = nullptr;

		if (OriginalParentComp)
		{
			if (USCS_Node** FoundNode = OriginalToNewNodeMap.Find(OriginalParentComp))
			{
				NewParentNode = *FoundNode;
			}
		}

		if(NewParentNode == nullptr)
		{
			NewParentNode = SCS->GetDefaultSceneRootNode();
		}
		NewNode->SetParent(NewParentNode);

		USceneComponent* NewSceneComp = Cast<USceneComponent>(NewNode->ComponentTemplate);
		if (NewSceneComp)
		{
			NewSceneComp->SetRelativeTransform(OriginalSceneComp->GetRelativeTransform());
		}
	}

	FBlueprintEditorUtils::MarkBlueprintAsStructurallyModified(NewBlueprint);
	FKismetEditorUtilities::CompileBlueprint(NewBlueprint);

	NewBlueprint->MarkPackageDirty();

	const FString PackageFileName = FPackageName::LongPackageNameToFilename(UniquePackageName, FPackageName::GetAssetPackageExtension());
	FSavePackageArgs SaveArgs;
	SaveArgs.TopLevelFlags = RF_Standalone;
	SaveArgs.SaveFlags = SAVE_NoError;
	UPackage::SavePackage(NewBlueprint->GetPackage(), nullptr, *PackageFileName, SaveArgs);
#endif
}

void ALevelScriptActor_Generate::GeneratePrefabFromFolder()
{
	if (TargetFolderPath.IsEmpty() || NewPrefabPath.IsEmpty()) return;

	TArray<AActor*> ActorsInFolder = GetActorsInFolder(TargetFolderPath);
	if (ActorsInFolder.Num() == 0) return;

	CreatePrefabFromActors(ActorsInFolder, NewPrefabPath);
}

#if WITH_EDITOR
void ALevelScriptActor_Generate::FocusEditorCameraOnActors(const TArray<AActor*>& inActors)
{
	if (inActors.Num() <= 0) return;

	FBox BoundingBox(ForceInit);
	for (AActor* Actor : inActors)
	{
		if (Actor) BoundingBox += Actor->GetComponentsBoundingBox(true);
	}

	if (!IsValid(GEditor)) return;

	FViewport* viewport = GEditor->GetActiveViewport();
	if (!viewport) return;

	FEditorViewportClient* editorViewport = (FEditorViewportClient*)viewport->GetClient();
	if (!editorViewport) return;

	editorViewport->FocusViewportOnBox(BoundingBox);
}
#endif