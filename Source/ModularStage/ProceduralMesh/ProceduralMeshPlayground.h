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

	/** Generates a simple box mesh with the given extents. */
	void GenerateBox(FVector InExtent);

	/** Converts the current procedural mesh to a static mesh asset. */
	UStaticMesh* ConvertToStaticMesh(FString AssetPath, FString AssetName);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ProceduralMeshPlayground")
	UProceduralMeshComponent* ProceduralMesh;

private:
	FMeshDescription BuildMeshDescription(UProceduralMeshComponent* ProcMeshComp);
	TMap<UMaterialInterface*, FPolygonGroupID> BuildMaterialMap(UProceduralMeshComponent* ProcMeshComp, FMeshDescription& MeshDescription);
};