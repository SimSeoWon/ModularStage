// Copyright Epic Games, Inc. All Rights Reserved.

#include "SPreviewViewport.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

//--------------------------------------------------------------------
// FPreviewViewportClient
//--------------------------------------------------------------------

FPreviewViewportClient::FPreviewViewportClient(FPreviewScene& InPreviewScene, const TWeakPtr<SEditorViewport>& InEditorViewportWidget)
	: FEditorViewportClient(nullptr, &InPreviewScene, InEditorViewportWidget)
{
	// Setup defaults for the viewport.
	SetRealtime(true);
	SetViewMode(VMI_Lit);
	EngineShowFlags.SetSnap(0);
	EngineShowFlags.CompositeEditorPrimitives = true;
	OverrideNearClipPlane(1.0f);
	bUsingOrbitCamera = true;
}

void FPreviewViewportClient::Tick(float DeltaSeconds)
{
	FEditorViewportClient::Tick(DeltaSeconds);

	// Tick the preview scene world.
	if (!GIntraFrameDebuggingGameThread)
	{
		PreviewScene->GetWorld()->Tick(LEVELTICK_All, DeltaSeconds);
	}
}

//--------------------------------------------------------------------
// SPreviewViewport
//--------------------------------------------------------------------

SPreviewViewport::~SPreviewViewport()
{
	if (ViewportClient.IsValid())
	{
		ViewportClient->Viewport = nullptr;
	}
}

void SPreviewViewport::Construct(const FArguments& InArgs)
{
	// Create a preview scene.
	PreviewScene = MakeShareable(new FPreviewScene(FPreviewScene::ConstructionValues()));

	// Create a static mesh component to display in the scene.
	PreviewMeshComponent = NewObject<UStaticMeshComponent>();
	
	UStaticMesh* CubeMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Engine/BasicShapes/Cube"));
	if (CubeMesh)
	{
		PreviewMeshComponent->SetStaticMesh(CubeMesh);
	}

	PreviewScene->AddComponent(PreviewMeshComponent, FTransform::Identity);

	// Construct the base SEditorViewport, which will call MakeEditorViewportClient().
	SEditorViewport::Construct(SEditorViewport::FArguments());
}

TSharedRef<FEditorViewportClient> SPreviewViewport::MakeEditorViewportClient()
{
	// Create and configure the custom viewport client.
	ViewportClient = MakeShareable(new FPreviewViewportClient(*PreviewScene, SharedThis(this)));

	ViewportClient->SetViewLocation(FVector(75.0f, 75.0f, 75.0f));
	ViewportClient->SetViewRotation(FRotator(-45.0f, 45.0f, 0.0f));

	return ViewportClient.ToSharedRef();
}
