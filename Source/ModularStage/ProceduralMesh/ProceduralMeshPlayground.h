// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MeshDescription.h"
#include "ProceduralMeshPlayground.generated.h"
class UProceduralMeshComponent;

UCLASS()
class MODULARSTAGE_API AProceduralMeshPlayground : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProceduralMeshPlayground(const FObjectInitializer& ObjectInitializer);

	/** Generates a simple plane mesh with the given extents. */
	UFUNCTION(CallInEditor, Category = "Procedural Mesh")
	void GeneratePlane();

	/** Converts the current procedural mesh to a static mesh asset. */
	UFUNCTION(CallInEditor, Category = "Procedural Mesh")
	void ConvertToStaticMesh();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Mesh")
	FVector PlaneExtent = FVector(100.f, 100.f, 0.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Mesh")
	FString StaticMeshAssetPath = TEXT("/Game/ProceduralMeshes");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Mesh")
	FString StaticMeshAssetName = TEXT("MyProceduralMesh");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Mesh|Generation")
	TArray<FIntPoint> DisabledTiles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Procedural Mesh|Generation", meta = (ClampMin = "1"))
	int32 GridResolution = 10;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProceduralMeshPlayground")
	UProceduralMeshComponent* ProceduralMesh;

private:
	FMeshDescription BuildMeshDescription(UProceduralMeshComponent* ProcMeshComp);
	TMap<UMaterialInterface*, FPolygonGroupID> BuildMaterialMap(UProceduralMeshComponent* ProcMeshComp, FMeshDescription& MeshDescription);
};