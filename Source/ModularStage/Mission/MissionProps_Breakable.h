#pragma once

#include "CoreMinimal.h"
#include "Mission/MissionPropsBase.h"
#include "Interface/Hit_Interface.h"
#include "MissionProps_Breakable.generated.h"


class UGeometryCollectionComponent;
class UGeometryCollection;
class UShapeComponent;

UCLASS()
class MODULARSTAGE_API AMissionProps_Breakable : public AMissionPropsBase, public IHit_Interface
{
	GENERATED_BODY()

public:
    AMissionProps_Breakable();

    virtual void OnHit_Implementation(const FHitData& HitData);
protected:
    virtual void BeginPlay() override;

    // =================================================================
    // Component Structure
    // =================================================================

    // [Root] Location reference point (lightweight SceneComponent recommended)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<USceneComponent> SceneComponent;

    // [Container] Parent component for designers to place collisions
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<USceneComponent> CollisionParentComponent;

    // [Main] The actual destructible geometry collection
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UGeometryCollectionComponent> GeometryCollection;

    // =================================================================
    // Internal Data
    // =================================================================

    // Cached list of collisions placed by designers
    UPROPERTY(Transient)
    TArray<TObjectPtr<UShapeComponent>> ArrCollisionList;

    // Caching the original asset for restoration
    UPROPERTY(Transient)
    TObjectPtr<UGeometryCollection> SourceAsset;

    UPROPERTY(Transient, Replicated)
    bool bIsBroken = false;

    FTransform SavedTransform;
    FName AttachSocketName;

public:
    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

    // Destruction event (NetMulticast)
    UFUNCTION(NetMulticast, Reliable)
    void Multicast_OnBroken(const FVector& HitLocation, const FVector& HitNormal);

    // Restoration function
    UFUNCTION(BlueprintCallable, Category = "Interaction")
    void ResetGeometryCollection();
};
