// Fill out your copyright notice in the Description page of Project Settings.

#include "ProceduralMeshPlayground.h"
#include "PhysicsEngine/BodySetup.h"
#include "StaticMeshAttributes.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Materials/Material.h"
#include "ProceduralMeshComponent.h"
#include "MaterialDomain.h"
#include "Engine/StaticMeshSourceData.h"
#include "Engine/StaticMesh.h"
#include "UObject/Package.h"

// Sets default values
AProceduralMeshPlayground::AProceduralMeshPlayground(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMesh"));
    RootComponent = ProceduralMesh;
}

void AProceduralMeshPlayground::GenerateBox(FVector InExtent)
{
    if (!ProceduralMesh)
    {
        return;
    }

    TArray<FVector> Vertices;
    TArray<int32> Triangles;
    TArray<FVector> Normals;
    TArray<FVector2D> UVs;
    TArray<FProcMeshTangent> Tangents;
    TArray<FLinearColor> VertexColors; // Unused, but required by CreateMeshSection_LinearColor

    // Define the 8 vertices of the box
    FVector V0 = FVector(-InExtent.X, -InExtent.Y, -InExtent.Z);
    FVector V1 = FVector(InExtent.X, -InExtent.Y, -InExtent.Z);
    FVector V2 = FVector(InExtent.X, InExtent.Y, -InExtent.Z);
    FVector V3 = FVector(-InExtent.X, InExtent.Y, -InExtent.Z);
    FVector V4 = FVector(-InExtent.X, -InExtent.Y, InExtent.Z);
    FVector V5 = FVector(InExtent.X, -InExtent.Y, InExtent.Z);
    FVector V6 = FVector(InExtent.X, InExtent.Y, InExtent.Z);
    FVector V7 = FVector(-InExtent.X, InExtent.Y, InExtent.Z);

    Vertices.Append({ V0, V1, V2, V3, V4, V5, V6, V7 });

    // Define the 12 triangles (2 for each of the 6 faces)
    // Front face
    Triangles.Append({0, 2, 1, 0, 3, 2});
    // Back face
    Triangles.Append({4, 5, 6, 4, 6, 7});
    // Left face
    Triangles.Append({0, 7, 3, 0, 4, 7});
    // Right face
    Triangles.Append({1, 2, 6, 1, 6, 5});
    // Top face
    Triangles.Append({3, 7, 6, 3, 6, 2});
    // Bottom face
    Triangles.Append({0, 1, 5, 0, 5, 4});

    // Normals and UVs are generated automatically by BuildMeshDescription from the vertex positions and triangles.
    // We can leave the normal/uv arrays empty here.

    ProceduralMesh->ClearAllMeshSections();
    ProceduralMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, TArray<FVector>(), TArray<FVector2D>(), VertexColors, Tangents, true);
}

UStaticMesh* AProceduralMeshPlayground::ConvertToStaticMesh(FString AssetPath, FString AssetName)
{
	if (!ProceduralMesh || ProceduralMesh->GetNumSections() == 0 || AssetName.IsEmpty() || AssetPath.IsEmpty())
	{
		return nullptr;
	}

    FString PackageName = AssetPath + "/" + AssetName;
	UPackage* Package = CreatePackage(*PackageName);
	check(Package);

	// Create StaticMesh object
	UStaticMesh* StaticMesh = NewObject<UStaticMesh>(Package, *AssetName, RF_Public | RF_Standalone);
	StaticMesh->InitResources();
	StaticMesh->SetLightingGuid();

	FMeshDescription MeshDescription = BuildMeshDescription(ProceduralMesh);

	// Add source to new StaticMesh
    FStaticMeshSourceModel& SrcModel = StaticMesh->AddSourceModel();
    SrcModel.BuildSettings.bRecomputeNormals = true;
    SrcModel.BuildSettings.bRecomputeTangents = true;
    SrcModel.BuildSettings.bRemoveDegenerates = true;
    SrcModel.BuildSettings.bUseHighPrecisionTangentBasis = false;
    SrcModel.BuildSettings.bUseFullPrecisionUVs = false;
    SrcModel.BuildSettings.bGenerateLightmapUVs = true;
    SrcModel.BuildSettings.SrcLightmapIndex = 0;
    SrcModel.BuildSettings.DstLightmapIndex = 1;
    StaticMesh->CreateMeshDescription(0, MoveTemp(MeshDescription));
    StaticMesh->CommitMeshDescription(0);

    // Collision
    StaticMesh->CreateBodySetup();
    UBodySetup* NewBodySetup = StaticMesh->GetBodySetup();
    NewBodySetup->BodySetupGuid = FGuid::NewGuid();
    NewBodySetup->CollisionTraceFlag = CTF_UseComplexAsSimple;
    NewBodySetup->CreatePhysicsMeshes();

    // Materials
    for (int32 SectionIdx = 0; SectionIdx < ProceduralMesh->GetNumSections(); SectionIdx++)
    {
        UMaterialInterface* Material = ProceduralMesh->GetMaterial(SectionIdx);
        if(Material)
        {
            StaticMesh->GetStaticMaterials().Add(FStaticMaterial(Material));
        }
    }

	//Set the Imported version before calling the build
	StaticMesh->ImportVersion = EImportStaticMeshVersion::LastVersion;

	// Build mesh from source
	StaticMesh->Build(false);
	StaticMesh->PostEditChange();

	// Notify asset registry of new asset
	FAssetRegistryModule::AssetCreated(StaticMesh);
    Package->MarkPackageDirty();

	return StaticMesh;
}


FMeshDescription AProceduralMeshPlayground::BuildMeshDescription(UProceduralMeshComponent* ProcMeshComp)
{
	FMeshDescription MeshDescription;

	FStaticMeshAttributes AttributeGetter(MeshDescription);
	AttributeGetter.Register();

	TPolygonGroupAttributesRef<FName> PolygonGroupNames = AttributeGetter.GetPolygonGroupMaterialSlotNames();
	TVertexAttributesRef<FVector3f> VertexPositions = AttributeGetter.GetVertexPositions();
	TVertexInstanceAttributesRef<FVector3f> Tangents = AttributeGetter.GetVertexInstanceTangents();
	TVertexInstanceAttributesRef<float> BinormalSigns = AttributeGetter.GetVertexInstanceBinormalSigns();
	TVertexInstanceAttributesRef<FVector3f> Normals = AttributeGetter.GetVertexInstanceNormals();
	TVertexInstanceAttributesRef<FVector4f> Colors = AttributeGetter.GetVertexInstanceColors();
	TVertexInstanceAttributesRef<FVector2f> UVs = AttributeGetter.GetVertexInstanceUVs();

	const int32 NumSections = ProcMeshComp->GetNumSections();
	int32 VertexCount = 0;
	int32 VertexInstanceCount = 0;
	int32 PolygonCount = 0;

	TMap<UMaterialInterface*, FPolygonGroupID> UniqueMaterials = BuildMaterialMap(ProcMeshComp, MeshDescription);
	TArray<FPolygonGroupID> PolygonGroupForSection;
	PolygonGroupForSection.Reserve(NumSections);

	for (int32 SectionIdx = 0; SectionIdx < NumSections; SectionIdx++)
	{
		FProcMeshSection* ProcSection = ProcMeshComp->GetProcMeshSection(SectionIdx);
		VertexCount += ProcSection->ProcVertexBuffer.Num();
		VertexInstanceCount += ProcSection->ProcIndexBuffer.Num();
		PolygonCount += ProcSection->ProcIndexBuffer.Num() / 3;
	}
	MeshDescription.ReserveNewVertices(VertexCount);
	MeshDescription.ReserveNewVertexInstances(VertexInstanceCount);
	MeshDescription.ReserveNewPolygons(PolygonCount);
	MeshDescription.ReserveNewEdges(PolygonCount * 2);
	UVs.SetNumChannels(4);

	for (int32 SectionIdx = 0; SectionIdx < NumSections; SectionIdx++)
	{
		UMaterialInterface* Material = ProcMeshComp->GetMaterial(SectionIdx);
		if (Material == nullptr)
		{
			Material = UMaterial::GetDefaultMaterial(MD_Surface);
		}
		FPolygonGroupID* PolygonGroupID = UniqueMaterials.Find(Material);
		check(PolygonGroupID != nullptr);
		PolygonGroupForSection.Add(*PolygonGroupID);
	}

	for (int32 SectionIdx = 0; SectionIdx < NumSections; SectionIdx++)
	{
		FProcMeshSection* ProcSection = ProcMeshComp->GetProcMeshSection(SectionIdx);
		FPolygonGroupID PolygonGroupID = PolygonGroupForSection[SectionIdx];
		TMap<int32, FVertexID> VertexIndexToVertexID;
		VertexIndexToVertexID.Reserve(ProcSection->ProcVertexBuffer.Num());
		for (int32 VertexIndex = 0; VertexIndex < ProcSection->ProcVertexBuffer.Num(); ++VertexIndex)
		{
			FProcMeshVertex& Vert = ProcSection->ProcVertexBuffer[VertexIndex];
			const FVertexID VertexID = MeshDescription.CreateVertex();
			VertexPositions[VertexID] = (FVector3f)Vert.Position;
			VertexIndexToVertexID.Add(VertexIndex, VertexID);
		}
		int32 NumIndices = ProcSection->ProcIndexBuffer.Num();
		for (int32 IndiceIndex = 0; IndiceIndex < NumIndices; IndiceIndex++)
		{
			const int32 VertexIndex = ProcSection->ProcIndexBuffer[IndiceIndex];
			const FVertexID VertexID = VertexIndexToVertexID[VertexIndex];
			const FVertexInstanceID VertexInstanceID = MeshDescription.CreateVertexInstance(VertexID);

			FProcMeshVertex& ProcVertex = ProcSection->ProcVertexBuffer[VertexIndex];

			Tangents[VertexInstanceID] = (FVector3f)ProcVertex.Tangent.TangentX;
			Normals[VertexInstanceID] = (FVector3f)ProcVertex.Normal;
			BinormalSigns[VertexInstanceID] = ProcVertex.Tangent.bFlipTangentY ? -1.f : 1.f;
			Colors[VertexInstanceID] = FLinearColor(ProcVertex.Color);
			UVs.Set(VertexInstanceID, 0, FVector2f(ProcVertex.UV0));
			UVs.Set(VertexInstanceID, 1, FVector2f(ProcVertex.UV1));
			UVs.Set(VertexInstanceID, 2, FVector2f(ProcVertex.UV2));
			UVs.Set(VertexInstanceID, 3, FVector2f(ProcVertex.UV3));
		}

		for (int32 TriIdx = 0; TriIdx < (ProcSection->ProcIndexBuffer.Num() / 3); TriIdx++)
		{
			TArray<FVertexInstanceID> VertexInstanceIDs;
			VertexInstanceIDs.SetNum(3);
			for (int32 CornerIndex = 0; CornerIndex < 3; ++CornerIndex)
			{
				const int32 IndiceIndex = (TriIdx * 3) + CornerIndex;
                VertexInstanceIDs[CornerIndex] = FVertexInstanceID(IndiceIndex);
			}
			MeshDescription.CreatePolygon(PolygonGroupID, VertexInstanceIDs);
		}
	}
	return MeshDescription;
}

TMap<UMaterialInterface*, FPolygonGroupID> AProceduralMeshPlayground::BuildMaterialMap(UProceduralMeshComponent* ProcMeshComp, FMeshDescription& MeshDescription)
{
	TMap<UMaterialInterface*, FPolygonGroupID> UniqueMaterials;
	const int32 NumSections = ProcMeshComp->GetNumSections();
	UniqueMaterials.Reserve(NumSections);

	FStaticMeshAttributes AttributeGetter(MeshDescription);
	TPolygonGroupAttributesRef<FName> PolygonGroupNames = AttributeGetter.GetPolygonGroupMaterialSlotNames();
	for (int32 SectionIdx = 0; SectionIdx < NumSections; SectionIdx++)
	{
		UMaterialInterface* Material = ProcMeshComp->GetMaterial(SectionIdx);
		if (Material == nullptr)
		{
			Material = UMaterial::GetDefaultMaterial(MD_Surface);
		}

		if (!UniqueMaterials.Contains(Material))
		{
			FPolygonGroupID NewPolygonGroup = MeshDescription.CreatePolygonGroup();
			UniqueMaterials.Add(Material, NewPolygonGroup);
			PolygonGroupNames[NewPolygonGroup] = Material->GetFName();
		}
	}
	return UniqueMaterials;
}