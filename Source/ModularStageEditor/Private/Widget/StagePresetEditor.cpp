#include "StagePresetEditor.h"
#include "Editor.h"
#include "UnrealEd.h"
#include "AssetToolsModule.h"
#include "Selection.h"
#include "EditorUtilityWidgetComponents.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Misc/PackageName.h"
#include "UObject/Package.h"
#include "Factories/BlueprintFactory.h"
#include "Engine/SimpleConstructionScript.h"
#include "UObject/SavePackage.h"
#include "BlueprintEditorLibrary.h"

#include "ModularStage/Mission/MissionPrefab.h"
//#include "ModularStage/LevelScriptActor_Generate.h"

void UStagePresetEditor::NativeConstruct()
{
	Super::NativeConstruct();

	Btn_CreatePrefab->OnClicked.AddDynamic(this, &UStagePresetEditor::OnClicked_Prefab_Components);

	//Btn_CreatePrefab2->OnClicked.AddDynamic(this, &UStagePresetEditor::OnClicked_Prefab_Components);
}

void UStagePresetEditor::GetSelectedActors(TArray<AActor*>& OutActors)
{
	OutActors.Empty();
	for (FSelectionIterator Iter(*GEditor->GetSelectedActors()); Iter; ++Iter)
	{
		if (AActor* actor = Cast<AActor>(*Iter))
		{
			OutActors.Add(actor);
		}
	}
}

void UStagePresetEditor::OnClicked_Prefab()
{
	TArray<AActor*> arrActors;
	GetSelectedActors(arrActors);

	if (arrActors.IsEmpty())
		return;

	FString savedPath = Edit_PackagePath->GetText().ToString();
	CreatePrefabFromActors(arrActors, savedPath);
}

void UStagePresetEditor::OnClicked_Prefab_Components()
{
	TArray<AActor*> arrActors;
	GetSelectedActors(arrActors);

	if (arrActors.IsEmpty())
		return;

	FString savedPath = Edit_PackagePath->GetText().ToString();
	CreatePrefabFromCollectedComponents(arrActors, savedPath);
}

void UStagePresetEditor::CreatePrefabFromActors(const TArray<AActor*>& inActors, const FString& inSavePath)
{
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

}

void UStagePresetEditor::CreatePrefabFromCollectedComponents(const TArray<AActor*>& InActors, const FString& inSavePath)
{
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

		if (NewParentNode == nullptr)
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

}