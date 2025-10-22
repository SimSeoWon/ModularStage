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

// !! �߿�: CalcBounds �Լ� ����
FBoxSphereBounds UMissionPropsComponent_Spawner::CalcBounds(const FTransform& LocalToWorld) const
{
    // ��� SpawnPoints�� PatrolPoints�� �����ϴ� FBox�� �����մϴ�.
    // ���� ���� �ϳ��� ���ٸ� ������Ʈ ��ġ�� �������� ���� �ڽ��� ����ϴ�.
    if (SpawnPoints.Num() == 0 && PatrolPoints.Num() == 0)
    {
        return FBoxSphereBounds(LocalToWorld.GetLocation(), FVector(10.0f), 10.0f);
    }

    // TArray<FVector>�� ��� ���̹Ƿ� FBox�� ��� ���� �߰��մϴ�.
    TArray<FVector> AllPoints = SpawnPoints;
    AllPoints.Append(PatrolPoints);

    // FBox(Points, Count) �����ڴ� TArray<FVector>�� ���� ���� �����Ƿ�,
    // FBox(EForceInit)�� �ʱ�ȭ�ϰ� �������� ���� �߰��մϴ�.
    FBox BoundingBox(EForceInit::ForceInit);
    for (const FVector& Point : AllPoints)
    {
        BoundingBox += Point;
    }

    // ���� �����̽��� BoundingBox�� World-Space FBoxSphereBounds�� ��ȯ�Ͽ� ��ȯ�մϴ�.
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