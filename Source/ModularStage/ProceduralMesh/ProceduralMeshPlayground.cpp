// Fill out your copyright notice in the Description page of Project Settings.

#include "ProceduralMeshPlayground.h"
#include "IContentBrowserSingleton.h"
#include "PhysicsEngine/BodySetup.h"
#include "StaticMeshAttributes.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Materials/Material.h"
#include "ProceduralMeshComponent.h"
#include "MaterialDomain.h"
#include "Engine/StaticMeshSourceData.h"
#include "Engine/StaticMesh.h"
#include "UObject/Package.h"
#include "ContentBrowserModule.h"
#include "AssetToolsModule.h"

// Sets default values
AProceduralMeshPlayground::AProceduralMeshPlayground(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ProceduralMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMesh"));
    RootComponent = ProceduralMesh;

    // Use the complex geometry of the mesh for collision.
    // This is crucial for the NavMesh to recognize the holes.
    ProceduralMesh->bUseComplexAsSimpleCollision = true;
}

void AProceduralMeshPlayground::GeneratePlane()
{
    if (!ProceduralMesh)
    {
        return;
    }

    ProceduralMesh->ClearAllMeshSections();

    TArray<FVector> Vertices;
    TArray<int32> Triangles;
    TArray<FVector> Normals; // Unreal will calculate these for us
    TArray<FVector2D> UVs; // We'll generate these
    TArray<FProcMeshTangent> Tangents; // Unreal will calculate these
    TArray<FLinearColor> VertexColors; // Not used

    const float CellSizeX = (PlaneExtent.X * 2.f) / GridResolution;
    const float CellSizeY = (PlaneExtent.Y * 2.f) / GridResolution;

    for (int32 Y_Index = 0; Y_Index < GridResolution; ++Y_Index)
    {
        for (int32 X_Index = 0; X_Index < GridResolution; ++X_Index)
        {
            // Check if the current tile index is in the disabled list
            if (DisabledTiles.Contains(FIntPoint(X_Index, Y_Index)))
            {
                // For disabled tiles, generate a tall box to block navigation
                const float StartX = -PlaneExtent.X + X_Index * CellSizeX;
                const float StartY = -PlaneExtent.Y + Y_Index * CellSizeY;
                const int32 BaseVertexIndex = Vertices.Num();

                // 8 vertices of the cube
                FVector V0 = FVector(StartX, StartY, 0);
                FVector V1 = FVector(StartX + CellSizeX, StartY, 0);
                FVector V2 = FVector(StartX + CellSizeX, StartY + CellSizeY, 0);
                FVector V3 = FVector(StartX, StartY + CellSizeY, 0);
                FVector V4 = FVector(StartX, StartY, WallHeight);
                FVector V5 = FVector(StartX + CellSizeX, StartY, WallHeight);
                FVector V6 = FVector(StartX + CellSizeX, StartY + CellSizeY, WallHeight);
                FVector V7 = FVector(StartX, StartY + CellSizeY, WallHeight);
                Vertices.Append({ V0, V1, V2, V3, V4, V5, V6, V7 });

                // 12 triangles of the cube
                // Bottom face
                Triangles.Append({ BaseVertexIndex, BaseVertexIndex + 1, BaseVertexIndex + 2 });
                Triangles.Append({ BaseVertexIndex, BaseVertexIndex + 2, BaseVertexIndex + 3 });
                // Top face
                Triangles.Append({ BaseVertexIndex + 4, BaseVertexIndex + 6, BaseVertexIndex + 5 });
                Triangles.Append({ BaseVertexIndex + 4, BaseVertexIndex + 7, BaseVertexIndex + 6 });
                // Front face
                Triangles.Append({ BaseVertexIndex + 0, BaseVertexIndex + 4, BaseVertexIndex + 5 });
                Triangles.Append({ BaseVertexIndex + 0, BaseVertexIndex + 5, BaseVertexIndex + 1 });
                // Back face
                Triangles.Append({ BaseVertexIndex + 3, BaseVertexIndex + 2, BaseVertexIndex + 6 });
                Triangles.Append({ BaseVertexIndex + 3, BaseVertexIndex + 6, BaseVertexIndex + 7 });
                // Left face
                Triangles.Append({ BaseVertexIndex + 0, BaseVertexIndex + 3, BaseVertexIndex + 7 });
                Triangles.Append({ BaseVertexIndex + 0, BaseVertexIndex + 7, BaseVertexIndex + 4 });
                // Right face
                Triangles.Append({ BaseVertexIndex + 1, BaseVertexIndex + 5, BaseVertexIndex + 6 });
                Triangles.Append({ BaseVertexIndex + 1, BaseVertexIndex + 6, BaseVertexIndex + 2 });

                // Add UVs for the 8 vertices
                for (int i = 0; i < 8; ++i) { UVs.Add(FVector2D(0,0)); } // Dummy UVs
            }
            else
            {
                // If not disabled, generate the quad for this cell
                const float StartX = -PlaneExtent.X + X_Index * CellSizeX;
                const float StartY = -PlaneExtent.Y + Y_Index * CellSizeY;
                const int32 BaseVertexIndex = Vertices.Num();

                // Define the 4 vertices of the quad
                FVector V0 = FVector(StartX, StartY, 0);
                FVector V1 = FVector(StartX + CellSizeX, StartY, 0);
                FVector V2 = FVector(StartX + CellSizeX, StartY + CellSizeY, 0);
                FVector V3 = FVector(StartX, StartY + CellSizeY, 0);
                Vertices.Append({ V0, V1, V2, V3 });

                // Define the 2 triangles for the quad
                Triangles.Append({ BaseVertexIndex, BaseVertexIndex + 2, BaseVertexIndex + 1 });
                Triangles.Append({ BaseVertexIndex, BaseVertexIndex + 3, BaseVertexIndex + 2 });

                // Add UVs corresponding to the vertices
                UVs.Add(FVector2D((StartX + PlaneExtent.X) / (PlaneExtent.X * 2.f), (StartY + PlaneExtent.Y) / (PlaneExtent.Y * 2.f)));
                UVs.Add(FVector2D((StartX + CellSizeX + PlaneExtent.X) / (PlaneExtent.X * 2.f), (StartY + PlaneExtent.Y) / (PlaneExtent.Y * 2.f)));
                UVs.Add(FVector2D((StartX + CellSizeX + PlaneExtent.X) / (PlaneExtent.X * 2.f), (StartY + CellSizeY + PlaneExtent.Y) / (PlaneExtent.Y * 2.f)));
                UVs.Add(FVector2D((StartX + PlaneExtent.X) / (PlaneExtent.X * 2.f), (StartY + CellSizeY + PlaneExtent.Y) / (PlaneExtent.Y * 2.f)));
            }
        }
    }

    if (Vertices.Num() > 0)
    {
        ProceduralMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, TArray<FVector>(), UVs, VertexColors, Tangents, true);
    }
}

void AProceduralMeshPlayground::ConvertToStaticMesh()
{
	if (!ProceduralMesh || ProceduralMesh->GetNumSections() == 0 || StaticMeshAssetName.IsEmpty() || StaticMeshAssetPath.IsEmpty())
	{
		return;
	}

	FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools");
	FString UniquePackageName;
	FString UniqueAssetName;
	AssetToolsModule.Get().CreateUniqueAssetName(StaticMeshAssetPath + TEXT("/") + StaticMeshAssetName, TEXT(""), UniquePackageName, UniqueAssetName);

	UPackage* Package = CreatePackage(*UniquePackageName);
	check(Package);

	// Create StaticMesh object
	UStaticMesh* StaticMesh = NewObject<UStaticMesh>(Package, *UniqueAssetName, RF_Public | RF_Standalone);
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

	// Sync content browser to the new asset
	FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
	ContentBrowserModule.Get().SyncBrowserToAssets(TArray<UObject*>{StaticMesh});

	
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

	int32 VertexInstanceOffset = 0;
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
                VertexInstanceIDs[CornerIndex] = FVertexInstanceID(VertexInstanceOffset + IndiceIndex);
			}
			MeshDescription.CreatePolygon(PolygonGroupID, VertexInstanceIDs);
		}

		VertexInstanceOffset += ProcSection->ProcIndexBuffer.Num();
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