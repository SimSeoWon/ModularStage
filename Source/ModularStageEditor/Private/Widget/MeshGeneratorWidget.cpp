// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/MeshGeneratorWidget.h"
#include "SPreviewViewport.h"
#include "Widgets/SBoxPanel.h"

#include "MissionPrefab.h"

void UMeshGeneratorWidget::SetTargetPrefab(AMissionPrefab* InPrefab)
{
    TargetPrefab = InPrefab;
    // We will need to add logic here to pass the prefab to the viewport.
}

TSharedRef<SWidget> UMeshGeneratorWidget::RebuildWidget()
{
    // Create the preview viewport widget.
    SAssignNew(PreviewViewportWidget, SPreviewViewport);

    // Return the viewport widget wrapped in a vertical box.
    return SNew(SVerticalBox)
        + SVerticalBox::Slot()
        .FillHeight(1.0f)
        [
            PreviewViewportWidget.ToSharedRef()
        ];
}
