// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/MeshGeneratorWidget.h"
#include "SPreviewViewport.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SScrollBox.h"
#include "AdvancedPreviewScene.h"
#include "Engine/Blueprint.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Widgets/Input/SNumericEntryBox.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"
#include "ProceduralMesh/ProceduralMeshPlayground.h"
#include "UI/Square/SquareTileView.h"

#include "MissionPrefab.h"

void UMeshGeneratorWidget::SetTargetPrefab(AMissionPrefab* InPrefab)
{
    TargetPrefab = InPrefab;
    // We will need to add logic here to pass the prefab to the viewport.
}

TSharedRef<SWidget> UMeshGeneratorWidget::RebuildWidget()
{
    if (!PreviewScene.IsValid())
    {
        PreviewScene = MakeShareable(new FAdvancedPreviewScene(FPreviewScene::ConstructionValues()));
        PreviewScene->SetFloorVisibility(true);
    }

    // Load the blueprint and spawn the actor
    const FString BlueprintPath = TEXT("/Game/Blueprints/BP_ProceduralMesh.BP_ProceduralMesh");
    UBlueprint* Blueprint = LoadObject<UBlueprint>(nullptr, *BlueprintPath);

    if (Blueprint && Blueprint->GeneratedClass)
    {
        UWorld* World = PreviewScene->GetWorld();
        if (World)
        {
            if (PreviewActor.IsValid())
            {
                PreviewActor->Destroy();
            }

            AActor* SpawnedActor = World->SpawnActor<AActor>(Blueprint->GeneratedClass, FTransform::Identity);
            PreviewActor = SpawnedActor;
        }
    }

    // Create the main horizontal box
    TSharedRef<SHorizontalBox> MainHorizontalBox = SNew(SHorizontalBox)
        + SHorizontalBox::Slot()
        .FillWidth(1.0f)
        [
            SNew(SBox)
            .MinDesiredWidth(600.0f)
            [
                SAssignNew(PreviewViewportWidget, SPreviewViewport)
                .PreviewScene(PreviewScene)
            ]
        ]
		+ SHorizontalBox::Slot()
		.AutoWidth()
		[
			SNew(SBox)
			.WidthOverride(600.0f)
			[
				SNew(SVerticalBox)

				// Num Squares Label
				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(5)
				[
					SNew(STextBlock).Text(FText::FromString(TEXT("Number of Squares")))
				]

				// Num Squares Input
				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(5)
				[
					SNew(SNumericEntryBox<int32>)
					.Value_Lambda([this] { return NumSquares; })
					.OnValueChanged_Lambda([this](int32 NewValue) { NumSquares = NewValue; })
				]

				// Size Label
				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(5)
				[
					SNew(STextBlock).Text(FText::FromString(TEXT("Square Size")))
				]

				// Size Input
				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(5)
				[
					SNew(SNumericEntryBox<float>)
					.Value_Lambda([this] { return SquareSize; })
					.OnValueChanged_Lambda([this](float NewValue) { SquareSize = NewValue; })
				]

				// Prefab Path Label
				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(5)
				[
					SNew(STextBlock).Text(FText::FromString(TEXT("Mission Prefab Path")))
				]

				// Prefab Path Input
				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(5)
				[
					SNew(SHorizontalBox)
					+ SHorizontalBox::Slot()
					.FillWidth(1.f)
					[
						SNew(SEditableTextBox)
						.Text(FText::FromString(PrefabPath))
						.OnTextChanged_Lambda([this](const FText& NewText) { PrefabPath = NewText.ToString(); })
					]
					+ SHorizontalBox::Slot()
					.AutoWidth()
					.Padding(2, 0, 0, 0)
					[
						SNew(SButton)
						.Text(FText::FromString(TEXT("...")))
						.OnClicked(FOnClicked::CreateUObject(this, &UMeshGeneratorWidget::OnBrowsePrefabPathClicked))
					]
				]

				// Load Prefab Button
				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(5)
				[
					SNew(SButton)
					.Text(FText::FromString(TEXT("Load Prefab")))
					.OnClicked(FOnClicked::CreateUObject(this, &UMeshGeneratorWidget::OnLoadPrefabClicked))
				]

				// Generate Button
				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(5)
				[
					SNew(SButton)
					.Text(FText::FromString(TEXT("Generate Mesh")))
					.OnClicked(FOnClicked::CreateUObject(this, &UMeshGeneratorWidget::OnGenerateMeshClicked))
				]

				// Export Button
				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(5)
				[
					SNew(SButton)
					.Text(FText::FromString(TEXT("Export to Static Mesh")))
					.OnClicked(FOnClicked::CreateUObject(this, &UMeshGeneratorWidget::OnExportMeshClicked))
				]

				// Add the SquareTileView
				+ SVerticalBox::Slot()
				.FillHeight(1.0f)
				.Padding(5)
				[
					SNew(SScrollBox)
					+ SScrollBox::Slot()
					[
						SNew(SBox)
						.Content()
						[
							SquareTileViewClass ? (SquareTileView = CreateWidget<USquareTileView>(this, SquareTileViewClass))->TakeWidget() : SNullWidget::NullWidget
						]
					]
				]
			]
		];

    // Initialize the SquareTileView after it has been created
    if (SquareTileView)
    {
        SquareTileView->SetTileListItems(SquareSize, 10, NumSquares * NumSquares, 1.0f);
    }

    return SNew(SScrollBox)
		.Orientation(Orient_Horizontal)
        + SScrollBox::Slot()
        [
            MainHorizontalBox
        ];
}


FReply UMeshGeneratorWidget::OnLoadPrefabClicked()
{
	// User will implement asset loading logic here based on the PrefabPath string.
	return FReply::Handled();
}

FReply UMeshGeneratorWidget::OnBrowsePrefabPathClicked()
{
	// Asset picker logic will be implemented here.
	return FReply::Handled();
}

FReply UMeshGeneratorWidget::OnGenerateMeshClicked()
{
    if (PreviewActor.IsValid())
    {
        if (AProceduralMeshPlayground* MeshActor = Cast<AProceduralMeshPlayground>(PreviewActor.Get()))
        {
            MeshActor->GridResolution = NumSquares;
            MeshActor->PlaneExtent = FVector(SquareSize, SquareSize, 0.f);
            MeshActor->GeneratePlane();
        }
    }

    return FReply::Handled();
}

FReply UMeshGeneratorWidget::OnExportMeshClicked()
{
    if (PreviewActor.IsValid())
    {
        if (AProceduralMeshPlayground* MeshActor = Cast<AProceduralMeshPlayground>(PreviewActor.Get()))
        {
            MeshActor->ConvertToStaticMesh();
        }
    }

    return FReply::Handled();
}

void UMeshGeneratorWidget::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	PreviewViewportWidget.Reset();
	PreviewScene.Reset();
}
