// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/MeshGeneratorWidget.h"
#include "Components/StaticMeshComponent.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "AssetToolsModule.h"
#include "EditorViewportClient.h"
#include "Framework/Application/SlateApplication.h"
#include "Misc/PackageName.h"
#include "EditorDirectories.h"
#include "PreviewScene.h"
#include "SEditorViewport.h"
#include "ModularStage/Mission/MissionPrefab.h"
#include "ModularStage/ProceduralMesh/ProceduralMeshPlayground.h"

UMeshGeneratorWidget::UMeshGeneratorWidget()
{
	//PreviewScene = MakeUnique<FPreviewScene>(FPreviewScene::ConstructionValues());
}

void UMeshGeneratorWidget::SetTargetPrefab(AMissionPrefab* InPrefab)
{
    TargetPrefab = InPrefab;
	SpawnTargetPrefab();
}

TSharedRef<SWidget> UMeshGeneratorWidget::RebuildWidget()
{
	return Super::RebuildWidget();
	/*if (IsDesignTime())
	{
		return Super::RebuildWidget();
	}*/

	/*LevelViewportClient = MakeShareable(new FEditorViewportClient(nullptr));

	ViewportWidget = SNew(SEditorViewport)
		.IsEnabled(FSlateApplication::Get().GetNormalExecutionAttribute())
		.bEnableGameViewport(true)
		.bRealtime(true)
		.ViewportClient(LevelViewportClient);

	LevelViewportClient->Viewport = ViewportWidget.Get();
	LevelViewportClient->SetRealtime(true);
	LevelViewportClient->SetViewportType(LVT_Perspective);
	LevelViewportClient->SetViewLocation(FVector(75, 75, 75));
	LevelViewportClient->SetViewRotation(FRotator(-45, -45, -45));
	LevelViewportClient->SetScene(PreviewScene.Get());

	return SNew(SVerticalBox)
		+ SVerticalBox::Slot()
		.FillHeight(1.0f)
		[
			ViewportWidget.ToSharedRef()
		];*/
}

void UMeshGeneratorWidget::BeginDestroy()
{
	Super::BeginDestroy();
	if (LevelViewportClient.IsValid())
	{
		LevelViewportClient->Viewport = nullptr;
	}
}

void UMeshGeneratorWidget::GenerateMesh()
{
    if (!TargetPrefab.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("Target Prefab is not valid."));
        return;
    }

    // 1. Get the properties from the TargetPrefab (e.g., ObstacleExtent).
    //    (This part will be implemented after AMissionPrefab is updated)

    // 2. Spawn a temporary AProceduralMeshPlayground actor.
    UWorld* World = GetWorld();
    if (!World)
    {
        return;
    }
    AProceduralMeshPlayground* Playground = World->SpawnActor<AProceduralMeshPlayground>();
    if (!Playground)
    {
        return;
    }

    // 3. Call GenerateBox on the playground actor.
    //    Playground->GenerateBox(TargetPrefab->ObstacleExtent);

    // 4. Prompt the user for a location to save the new StaticMesh asset.
    //    This requires using AssetToolsModule and SDlgPickAssetPath, which is a more involved UI operation.
    //    For now, we will log that the function was called.

    UE_LOG(LogTemp, Log, TEXT("GenerateMesh function called for prefab: %s"), *TargetPrefab->GetName());

    // 5. Convert the procedural mesh to a UStaticMesh asset.
    //    UStaticMesh* NewMesh = Playground->ConvertToStaticMesh(); // This function will need to be modified to handle asset creation.

    // 6. Assign the new mesh back to the prefab's component.
    //    TargetPrefab->GeneratedMeshComponent->SetStaticMesh(NewMesh);

    // 7. Clean up the temporary playground actor.
    Playground->Destroy();
}

void UMeshGeneratorWidget::SpawnTargetPrefab()
{
	if (!TargetPrefab.IsValid())
	{
		return;
	}

	// Clear existing components
	//PreviewScene->RemoveAllComponents();

	// Spawn the prefab's components
	for (UActorComponent* Component : TargetPrefab->GetComponents())
	{
		if (UStaticMeshComponent* StaticMeshComponent = Cast<UStaticMeshComponent>(Component))
		{
			UStaticMeshComponent* PreviewComponent = NewObject<UStaticMeshComponent>(GetTransientPackage());
			PreviewComponent->SetStaticMesh(StaticMeshComponent->GetStaticMesh());
			PreviewComponent->SetRelativeTransform(StaticMeshComponent->GetRelativeTransform());
			PreviewScene->AddComponent(PreviewComponent, PreviewComponent->GetRelativeTransform());
		}
	}
}
