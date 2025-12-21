// Copyright Epic Games, Inc. All Rights Reserved.

#include "MissionPropsComponent_Spawner.h"
#include "Engine/CollisionProfile.h"
#include "FSpawnerSceneProxy.h"

#if WITH_EDITOR
#include "ModularStageEditor/Public/Widget/EditorWidget_AssetSelector.h"
#include "ModularStageEditor/Public/Widget/EditorWidgetBase.h"
#endif

UMissionPropsComponent_Spawner::UMissionPropsComponent_Spawner()
{
	PrimaryComponentTick.bCanEverTick = true;

	SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
	bUseEditorCompositing = true;
	//bIsEditorOnly = true;
}

FBoxSphereBounds UMissionPropsComponent_Spawner::CalcBounds(const FTransform& LocalToWorld) const
{
    if (SpawnPoints.Num() == 0 && PatrolPoints.Num() == 0)
    {
        return FBoxSphereBounds(LocalToWorld.GetLocation(), FVector(10.0f), 10.0f);
    }

    TArray<FVector> AllPoints = SpawnPoints;
    AllPoints.Append(PatrolPoints);

    FBox BoundingBox(EForceInit::ForceInit);
    for (const FVector& Point : AllPoints)
    {
        BoundingBox += Point;
    }

    return FBoxSphereBounds(BoundingBox.TransformBy(LocalToWorld));
}

FPrimitiveSceneProxy* UMissionPropsComponent_Spawner::CreateSceneProxy()
{
    UE_LOG(LogTemp, Warning, TEXT("UMissionPropsComponent_Spawner::CreateSceneProxy() called!"));
    return new FSpawnerSceneProxy(this);
}

void UMissionPropsComponent_Spawner::BeginPlay()
{
	Super::BeginPlay();
}

void UMissionPropsComponent_Spawner::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UMissionPropsComponent_Spawner::OnActivate()
{
	// Implementation
}

void UMissionPropsComponent_Spawner::OnDeactivate()
{
	// Implementation
}

void UMissionPropsComponent_Spawner::ShowAsserBrowser()
{
#if WITH_EDITOR
	UEditorWidget_AssetSelector* editor = UEditorWidgetBase::ShowEditorUtilityWidget<UEditorWidget_AssetSelector>();
	if (false == IsValid(editor))
		return;

    editor->GetOnAcceptAssetPath().BindUObject(this, &UMissionPropsComponent_Spawner::OnSelectAssetPath);
	editor->Run();
#endif
}

void UMissionPropsComponent_Spawner::OnSelectAssetPath(FString inPath)
{

}
