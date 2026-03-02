#include "FHexGridSceneProxy.h"
#include "HexGridVisualComponent.h"
#include "MissionPrefab.h"
#include "SceneManagement.h"

FHexGridSceneProxy::FHexGridSceneProxy(UHexGridVisualComponent* InComponent)
	: FPrimitiveSceneProxy(InComponent)
{
	// 컴포넌트로부터 타일 설정 복사
	TileConfigs = InComponent->TileConfigs;

	// 부모 프리팹으로부터 인게임 표시 여부 확인
	AMissionPrefab* ParentPrefab = Cast<AMissionPrefab>(InComponent->GetOwner());
	if (ParentPrefab)
	{
		bShowInGame = ParentPrefab->bShowHexGridInGame;
	}
}

FPrimitiveViewRelevance FHexGridSceneProxy::GetViewRelevance(const FSceneView* View) const
{
	const bool bVisible = bShowInGame || View->Family->EngineShowFlags.Editor;

	FPrimitiveViewRelevance Result;
	Result.bDrawRelevance = IsShown(View) && bVisible;
	Result.bDynamicRelevance = true;
	Result.bEditorPrimitiveRelevance = View->Family->EngineShowFlags.Editor;
	Result.bShadowRelevance = false;
	return Result;
}

void FHexGridSceneProxy::GetDynamicMeshElements(const TArray<const FSceneView*>& Views, const FSceneViewFamily& ViewFamily, uint32 VisibilityMap, FMeshElementCollector& Collector) const
{
	for (int32 viewIndex = 0; viewIndex < Views.Num(); viewIndex++)
	{
		if (VisibilityMap & (1 << viewIndex))
		{
			FPrimitiveDrawInterface* PDI = Collector.GetPDI(viewIndex);
			const FMatrix& localToWorld = GetLocalToWorld();

			for (const FHexTileConfig& tile : TileConfigs)
			{
				if (!tile.bIsActive) continue;

				// 타일 중심점 (로컬 -> 월드 변환)
				FVector hexLocalCenter = UHexGridLibrary::HexToWorld(tile.Coord);
				FVector hexWorldCenter = localToWorld.TransformPosition(hexLocalCenter);

				// 색상 선택
				FLinearColor drawColor = FLinearColor::White;
				switch (tile.Role)
				{
				case EHexTileRole::Floor: drawColor = FLinearColor::Green; break;
				case EHexTileRole::Wall:  drawColor = FLinearColor::Red; break;
				case EHexTileRole::Entry: drawColor = FLinearColor::White; break;
				case EHexTileRole::Exit:  drawColor = FLinearColor::Blue; break;
				case EHexTileRole::Event: drawColor = FLinearColor::Yellow; break;
				}
				// 육각형 그리기 (6개 선)
				// [단일화] Pointy Top 표준에 맞게 30도 회전 오프셋을 적용하여 중심점 수식과 일치시킵니다.
				const float size = ModularStageDefine::HexGrid::TILE_SIZE;
				for (int32 i = 0; i < 6; ++i)
				{
					float angleDeg = 60.0f * i + 30.0f;
					float nextAngleDeg = 60.0f * (i + 1) + 30.0f;

					// 모든 꼭짓점도 중심점의 Z값을 따르도록 하여 높이 데이터와 일치시킴
					FVector p1_Local = hexLocalCenter + FVector(FMath::Cos(FMath::DegreesToRadians(angleDeg)) * size, FMath::Sin(FMath::DegreesToRadians(angleDeg)) * size, 0.0f);
					FVector p2_Local = hexLocalCenter + FVector(FMath::Cos(FMath::DegreesToRadians(nextAngleDeg)) * size, FMath::Sin(FMath::DegreesToRadians(nextAngleDeg)) * size, 0.0f);
					FVector p1_World = localToWorld.TransformPosition(p1_Local);
					FVector p2_World = localToWorld.TransformPosition(p2_Local);

					PDI->DrawLine(p1_World, p2_World, drawColor, SDPG_World, 2.0f);
				}
			}
		}
	}
}

SIZE_T FHexGridSceneProxy::GetTypeHash() const
{
	static size_t UniquePointer;
	return reinterpret_cast<size_t>(&UniquePointer);
}