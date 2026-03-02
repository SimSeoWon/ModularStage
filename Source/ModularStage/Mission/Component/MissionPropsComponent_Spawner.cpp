// Copyright Epic Games, Inc. All Rights Reserved.

#include "MissionPropsComponent_Spawner.h"
#include "Engine/CollisionProfile.h"
#include "FSpawnerSceneProxy.h"
#include "Misc/Guid.h"

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

FGuid UMissionPropsComponent_Spawner::GetGuid() const
{
	return ComponentGuid;
}

void UMissionPropsComponent_Spawner::SetGuid(const FGuid& InGuid)
{
	ComponentGuid = InGuid;
}

void UMissionPropsComponent_Spawner::PostInitProperties()
{
	Super::PostInitProperties();
	if (!ComponentGuid.IsValid())
	{
		ComponentGuid = FGuid::NewGuid();
	}
}

FBoxSphereBounds UMissionPropsComponent_Spawner::CalcBounds(const FTransform& LocalToWorld) const
{
    AMissionPrefab* ParentPrefab = Cast<AMissionPrefab>(GetOwner());
    if (!ParentPrefab)
    {
        return FBoxSphereBounds(LocalToWorld.GetLocation(), FVector(10.0f), 10.0f);
    }

    FBox BoundingBox(EForceInit::ForceInit);

    // 스폰 지점 인덱스를 월드 좌표로 변환하여 바운딩 박스 계산
    for (int32 Index : SpawnTileIndices)
    {
        FVector RelativePos = ParentPrefab->GetTileLocation(Index);
        BoundingBox += RelativePos;
    }

    // 순찰 지점 인덱스도 포함
    for (int32 Index : PatrolTileIndices)
    {
        FVector RelativePos = ParentPrefab->GetTileLocation(Index);
        BoundingBox += RelativePos;
    }

    // 데이터가 없는 경우 기본값
    if (!BoundingBox.IsValid)
    {
        return FBoxSphereBounds(LocalToWorld.GetLocation(), FVector(50.0f), 50.0f);
    }

    // 컴포넌트 위치(로컬) 기준의 바운딩 박스를 월드 좌표로 변환
    return FBoxSphereBounds(BoundingBox.TransformBy(LocalToWorld));
}

FPrimitiveSceneProxy* UMissionPropsComponent_Spawner::CreateSceneProxy()
{
    if (this->GetOwner() == nullptr)
    {
        return nullptr;
    }
    
    UE_LOG(LogTemp, Warning, TEXT("UMissionPropsComponent_Spawner::CreateSceneProxy() called for %s"), *GetName());
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
