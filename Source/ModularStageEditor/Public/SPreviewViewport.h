// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SEditorViewport.h"
#include "EditorViewportClient.h"
#include "PreviewScene.h"

/**
 * Custom Viewport Client for the preview viewport.
 */
class FPreviewViewportClient : public FEditorViewportClient
{
public:
	FPreviewViewportClient(FPreviewScene& InPreviewScene, const TWeakPtr<SEditorViewport>& InEditorViewportWidget);

	// FEditorViewportClient interface
	virtual void Tick(float DeltaSeconds) override;
};

/**
 * Implements a standalone preview viewport widget.
 */
class SPreviewViewport : public SEditorViewport
{
public:

	SLATE_BEGIN_ARGS(SPreviewViewport) { }
	SLATE_END_ARGS()

	/** Virtual destructor. */
	virtual ~SPreviewViewport();

	/**
	 * Constructs this widget.
	 *
	 * @param InArgs The Slate argument list.
	 */
	void Construct(const FArguments& InArgs);

protected:
	// SEditorViewport interface
	virtual TSharedRef<FEditorViewportClient> MakeEditorViewportClient() override;

private:

	/** The preview scene that we are viewing. */
	TSharedPtr<FPreviewScene> PreviewScene;

	/** The viewport client for the preview scene. */
	TSharedPtr<FPreviewViewportClient> ViewportClient;

	/** The static mesh component to display. */
	class UStaticMeshComponent* PreviewMeshComponent;
};
