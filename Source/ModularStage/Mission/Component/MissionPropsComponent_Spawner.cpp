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

// !! 중요: CalcBounds 함수 구현
FBoxSphereBounds UMissionPropsComponent_Spawner::CalcBounds(const FTransform& LocalToWorld) const
{
    // 모든 SpawnPoints와 PatrolPoints를 포함하는 FBox를 생성합니다.
    // 만약 점이 하나도 없다면 컴포넌트 위치를 기준으로 작은 박스를 만듭니다.
    if (SpawnPoints.Num() == 0 && PatrolPoints.Num() == 0)
    {
        return FBoxSphereBounds(LocalToWorld.GetLocation(), FVector(10.0f), 10.0f);
    }

    // TArray<FVector>를 사용 중이므로 FBox에 모든 점을 추가합니다.
    TArray<FVector> AllPoints = SpawnPoints;
    AllPoints.Append(PatrolPoints);

    // FBox(Points, Count) 생성자는 TArray<FVector>를 직접 받지 않으므로,
    // FBox(EForceInit)로 초기화하고 수동으로 점을 추가합니다.
    FBox BoundingBox(EForceInit::ForceInit);
    for (const FVector& Point : AllPoints)
    {
        BoundingBox += Point;
    }

    // 로컬 스페이스의 BoundingBox를 World-Space FBoxSphereBounds로 변환하여 반환합니다.
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

	editor->Run();
#endif
}