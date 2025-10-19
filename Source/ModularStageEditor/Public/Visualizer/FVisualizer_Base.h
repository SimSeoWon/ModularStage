// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ComponentVisualizer.h"

class FVisualizer_Base : public FComponentVisualizer
{
public:
	FVisualizer_Base();
	virtual ~FVisualizer_Base();

protected:
	// Helper functions for drawing
	void DrawWireBox(FPrimitiveDrawInterface* PDI, const FBox& Box, const FLinearColor& Color, ESceneDepthPriorityGroup DepthPriority = SDPG_World);
	void DrawWireCylinder(FPrimitiveDrawInterface* PDI, const FVector& Base, const FVector& X, const FVector& Y, const FVector& Z, float Radius, float HalfHeight, int32 NumSides, const FLinearColor& Color, ESceneDepthPriorityGroup DepthPriority = SDPG_World);
	void DrawWireSphere(FPrimitiveDrawInterface* PDI, const FVector& Center, const FLinearColor& Color, float Radius, int32 NumSides, ESceneDepthPriorityGroup DepthPriority = SDPG_World);
	void DrawCircle(FPrimitiveDrawInterface* PDI, const FVector& Center, const FVector& X, const FVector& Y, const FLinearColor& Color, float Radius, int32 NumSides, ESceneDepthPriorityGroup DepthPriority = SDPG_World);
};
