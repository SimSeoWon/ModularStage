// Copyright Epic Games, Inc. All Rights Reserved.

#include "FVisualizer_Base.h"
#include "SceneManagement.h"

FVisualizer_Base::FVisualizer_Base()
{
}

FVisualizer_Base::~FVisualizer_Base()
{
}

void FVisualizer_Base::DrawWireBox(FPrimitiveDrawInterface* PDI, const FBox& Box, const FLinearColor& Color, ESceneDepthPriorityGroup DepthPriority)
{
	if (PDI)
	{
		::DrawWireBox(PDI, Box, Color, SDPG_World);
	}
}

void FVisualizer_Base::DrawWireCylinder(FPrimitiveDrawInterface* PDI, const FVector& Base, const FVector& X, const FVector& Y, const FVector& Z, float Radius, float HalfHeight, int32 NumSides, const FLinearColor& Color, ESceneDepthPriorityGroup DepthPriority)
{
	if (PDI)
	{
		::DrawWireCylinder(PDI, Base, X, Y, Z, Color, Radius, HalfHeight, NumSides, DepthPriority);
	}
}

void FVisualizer_Base::DrawWireSphere(FPrimitiveDrawInterface* PDI, const FVector& Center, const FLinearColor& Color, float Radius, int32 NumSides, ESceneDepthPriorityGroup DepthPriority)
{
    if (PDI)
    {
        ::DrawWireSphere(PDI, Center, Color, Radius, NumSides, DepthPriority);
    }
}

void FVisualizer_Base::DrawCircle(FPrimitiveDrawInterface* PDI, const FVector& Center, const FVector& X, const FVector& Y, const FLinearColor& Color, float Radius, int32 NumSides, ESceneDepthPriorityGroup DepthPriority)
{
    if (PDI)
    {
        ::DrawCircle(PDI, Center, X, Y, Color, Radius, NumSides, DepthPriority);
    }
}
