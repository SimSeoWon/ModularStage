#include "Widget/SPrefabEditorViewport.h"
#include "Editor.h"
#include "UnrealEdGlobals.h"
#include "Editor/UnrealEdEngine.h"
#include "AssetEditorModeManager.h"
#include "UnrealWidget.h"
#include "HitProxies.h"
#include "EngineUtils.h"
#include "ComponentVisualizer.h" // 필수

#include "ModularStage/Mission/MissionPrefab.h"

//--------------------------------------------------------------------
// FPrefabEditorViewportClient
//--------------------------------------------------------------------

FPrefabEditorViewportClient::FPrefabEditorViewportClient(FAdvancedPreviewScene& InPreviewScene, const TWeakPtr<SEditorViewport>& InEditorViewportWidget)
	: FEditorViewportClient(nullptr, &InPreviewScene, InEditorViewportWidget)
{
	// Setup standard editor lighting and view
	SetRealtime(true);
	SetViewMode(VMI_Lit);
	EngineShowFlags.SetSnap(0);
	EngineShowFlags.CompositeEditorPrimitives = true;
	OverrideNearClipPlane(1.0f);
	
	// Default to perspective view
	SetViewportType(LVT_Perspective);
	SetViewLocation(FVector(500, 500, 500));
	SetViewRotation(FRotator(-45, 180 + 45, 0));

	// Enable Gizmo (Transform Widget)
	ShowWidget(true);
}

void FPrefabEditorViewportClient::Tick(float DeltaSeconds)
{
	FEditorViewportClient::Tick(DeltaSeconds);

	// Tick the preview world
	if (!GIntraFrameDebuggingGameThread && PreviewScene)
	{
		PreviewScene->GetWorld()->Tick(LEVELTICK_All, DeltaSeconds);
	}
}

void FPrefabEditorViewportClient::ProcessClick(FSceneView& View, HHitProxy* HitProxy, FKey Key, EInputEvent Event, uint32 HitX, uint32 HitY)
{
	// 1. 기존 비주얼라이저 처리 (비주얼라이저 포인트 선택 시 부모 호출 방지)
	bool bVisualizerHandled = false;

	if (HitProxy)
	{
		// 클릭된 녀석의 타입 이름을 출력
		UE_LOG(LogTemp, Warning, TEXT("HitProxy Type: %s"), HitProxy->GetType()->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("HitProxy is NULL (Background Clicked)"));
	}
	
	if (HitProxy && HitProxy->IsA(HComponentVisProxy::StaticGetType()))
	{
		HComponentVisProxy* VisProxy = (HComponentVisProxy*)HitProxy;
		if (VisProxy->Component.IsValid())
		{
			// 해당 컴포넌트의 비주얼라이저 찾기
			TSharedPtr<FComponentVisualizer> Visualizer = GUnrealEd->FindComponentVisualizer(VisProxy->Component->GetClass());
			if (Visualizer.IsValid())
			{
				// 클릭 처리 위임
				bVisualizerHandled = Visualizer->VisProxyHandleClick(this, VisProxy, FViewportClick(&View, this, Key, Event, HitX, HitY));

				if (bVisualizerHandled)
				{
					// 비주얼라이저가 처리를 했으면, 해당 컴포넌트를 추적 등록하고 선택합니다.
					UActorComponent* Component = const_cast<UActorComponent*>(VisProxy->Component.Get());
					VisualizedComponent = Component;
					SelectObject(Component, true);
				}
			}
		}
	}

	// 비주얼라이저가 처리하지 않았을 때만 기본 선택 로직 수행
	if (!bVisualizerHandled)
	{
		// 배경 클릭 시 비주얼라이저 포커스 해제
		if (HitProxy == nullptr)
		{
			VisualizedComponent.Reset();
		}

		FEditorViewportClient::ProcessClick(View, HitProxy, Key, Event, HitX, HitY);

		// ... (기존 선택 로직 유지) ...
		if (HitProxy && HitProxy->IsA(HActor::StaticGetType()))
		{
			// ...
		}
		else if (HitProxy == nullptr)
		{
			ClearSelection();
		}
	}
}

bool FPrefabEditorViewportClient::InputWidgetDelta(FViewport* InViewport, EAxisList::Type CurrentAxis, FVector& Drag, FRotator& Rot, FVector& Scale)
{
	bool bHandled = false;

	// 1. 비주얼라이저 입력 처리
	if (VisualizedComponent.IsValid() && CurrentAxis != EAxisList::Type::None)
	{
		TSharedPtr<FComponentVisualizer> Visualizer = GUnrealEd->FindComponentVisualizer(VisualizedComponent->GetClass());
		if (Visualizer.IsValid())
		{
			// HandleInputDelta 호출 (FVisualizer_Spawner::HandleInputDelta가 실행됨)
			bHandled = Visualizer->HandleInputDelta(this, InViewport, Drag, Rot, Scale);
		}
	}

	// 2. 비주얼라이저가 처리했으면 리턴, 아니면 기존 액터 이동 로직 수행
	if (bHandled)
	{
		Invalidate();
		return true;
	}

	// 기존 액터/컴포넌트 이동 로직
	if (CurrentAxis != EAxisList::Type::None && SelectedObjects.Num() > 0)
	{
		for (auto& WeakObject : SelectedObjects)
		{
			if (UObject* Obj = WeakObject.Get())
			{
				if (!IsValid(Obj)) continue;

				if (AActor* Actor = Cast<AActor>(Obj))
				{
					// Apply translation, rotation, and scale to Actor
					if (!Drag.IsZero())
					{
						Actor->AddActorWorldOffset(Drag);
					}
					if (!Rot.IsZero())
					{
						Actor->AddActorWorldRotation(Rot.Quaternion());
					}
					if (!Scale.IsZero())
					{
						Actor->SetActorScale3D(Actor->GetActorScale3D() + Scale);
					}
					bHandled = true;
				}
				else if (USceneComponent* Component = Cast<USceneComponent>(Obj))
				{
					// Apply translation, rotation, and scale to SceneComponent
					if (!Drag.IsZero())
					{
						Component->AddWorldOffset(Drag);
					}
					if (!Rot.IsZero())
					{
						Component->AddWorldRotation(Rot.Quaternion());
					}
					if (!Scale.IsZero())
					{
						Component->SetWorldScale3D(Component->GetComponentScale() + Scale);
					}
					bHandled = true;
				}
			}
		}
	}

	if (bHandled)
	{
		Invalidate();
	}

	return bHandled || FEditorViewportClient::InputWidgetDelta(InViewport, CurrentAxis, Drag, Rot, Scale);
}

void FPrefabEditorViewportClient::Draw(const FSceneView* View, FPrimitiveDrawInterface* PDI)
{
	FEditorViewportClient::Draw(View, PDI);

	// 1. Draw visualizers for all components of the root prefab actor
	if (PrefabActor.IsValid())
	{
		for (UActorComponent* Component : PrefabActor->GetComponents())
		{
			if (!IsValid(Component)) continue;

			TSharedPtr<FComponentVisualizer> Visualizer = GUnrealEd->FindComponentVisualizer(Component->GetClass());
			if (Visualizer.IsValid())
			{
				Visualizer->DrawVisualization(Component, View, PDI);
			}
		}
	}

	// 2. Draw selection highlights for currently selected objects
	if (SelectedObjects.Num() > 0)
	{
		for (auto& WeakObject : SelectedObjects)
		{
			if (UObject* Obj = WeakObject.Get())
			{
				if (AActor* Actor = Cast<AActor>(Obj))
				{
					if (IsValid(Actor))
					{
						FBox Box = Actor->GetComponentsBoundingBox(true);
						DrawWireBox(PDI, Box, FColor::Yellow, SDPG_World);
					}
				}
				else if (USceneComponent* Component = Cast<USceneComponent>(Obj))
				{
					if (IsValid(Component))
					{
						FBox ComponentBox = Component->Bounds.GetBox();
						DrawWireBox(PDI, ComponentBox, FColor::Cyan, SDPG_World);

						if (AActor* OwnerActor = Component->GetOwner())
						{
							FBox OwnerBox = OwnerActor->GetComponentsBoundingBox(true);
							DrawWireBox(PDI, OwnerBox, FColor::Yellow.WithAlpha(50), SDPG_World);
						}
					}
				}
			}
		}
	}
}

FVector FPrefabEditorViewportClient::GetWidgetLocation() const
{
	if (VisualizedComponent.IsValid())
	{
		TSharedPtr<FComponentVisualizer> Visualizer = GUnrealEd->FindComponentVisualizer(VisualizedComponent->GetClass());
		FVector VisualizerWidgetLocation;
		// 비주얼라이저가 위젯 위치를 제공한다면(포인트 선택 등) 그것을 반환
		if (Visualizer.IsValid() && Visualizer->GetWidgetLocation(this, VisualizerWidgetLocation))
		{
			return VisualizerWidgetLocation;
		}
	}

	if (SelectedObjects.Num() > 0)
	{
		for (auto& WeakObject : SelectedObjects)
		{
			if (UObject* Obj = WeakObject.Get())
			{
				if (AActor* Actor = Cast<AActor>(Obj))
				{
					return Actor->GetActorLocation();
				}
				else if (USceneComponent* Component = Cast<USceneComponent>(Obj))
				{
					return Component->GetComponentLocation();
				}
			}
		}
	}

	return FVector::ZeroVector;
}

FMatrix FPrefabEditorViewportClient::GetWidgetCoordSystem() const
{
	if (GetWidgetMode() == UE::Widget::WM_Translate || GetWidgetMode() == UE::Widget::WM_Rotate || GetWidgetMode() == UE::Widget::WM_Scale)
    {
        if (GetWidgetCoordSystemSpace() == COORD_Local)
        {
            if (SelectedObjects.Num() > 0)
            {
				for (auto& WeakObject : SelectedObjects)
				{
					if (UObject* Obj = WeakObject.Get())
					{
						FQuat Rotation = FQuat::Identity;
						if (AActor* Actor = Cast<AActor>(Obj))
						{
							Rotation = Actor->GetActorQuat();
						}
						else if (USceneComponent* Component = Cast<USceneComponent>(Obj))
						{
							Rotation = Component->GetComponentQuat();
						}
						return FQuatRotationTranslationMatrix(Rotation, FVector::ZeroVector);
					}
				}
            }
        }
    }
    return FMatrix::Identity;
}

UE::Widget::EWidgetMode FPrefabEditorViewportClient::GetWidgetMode() const
{
	bool bHasTransformableSelection = false;
	if (SelectedObjects.Num() > 0)
	{
		for (const auto& WeakObject : SelectedObjects)
		{
			if (UObject* Obj = WeakObject.Get())
			{
				if (Obj->IsA<AActor>() || Obj->IsA<USceneComponent>())
				{
					bHasTransformableSelection = true;
					break;
				}
			}
		}
	}

	if (bHasTransformableSelection)
	{
		return FEditorViewportClient::GetWidgetMode();
	}

	return UE::Widget::WM_None;
}

void FPrefabEditorViewportClient::SelectObject(UObject* InObject, bool bInReplaceSelection)
{
	if (bInReplaceSelection)
	{
		SelectedObjects.Empty();
	}

	if (InObject)
	{
		SelectedObjects.Add(InObject);
		
		UE_LOG(LogTemp, Log, TEXT("Selected Object: %s"), *InObject->GetName());
		
		// Notify selection change (broadcast the last selected object)
		OnSelectionChanged.ExecuteIfBound(InObject);

		// Ensure we are in a valid widget mode (default to Translate if None)
		if (GetWidgetMode() == UE::Widget::WM_None)
		{
			SetWidgetMode(UE::Widget::WM_Translate);
		}
	}
	
	Invalidate();
}

void FPrefabEditorViewportClient::ClearSelection()
{
	SelectedObjects.Empty();
	OnSelectionChanged.ExecuteIfBound(nullptr);
	Invalidate();
}

void FPrefabEditorViewportClient::SetPrefabActor(AMissionPrefab* InPrefabActor)
{
	PrefabActor = InPrefabActor;
}

//--------------------------------------------------------------------
// SPrefabEditorViewport
//--------------------------------------------------------------------

void SPrefabEditorViewport::Construct(const FArguments& InArgs, TSharedPtr<FAdvancedPreviewScene> InPreviewScene)
{
	PreviewScene = InPreviewScene;
	SEditorViewport::Construct(SEditorViewport::FArguments());
}

SPrefabEditorViewport::~SPrefabEditorViewport()
{
	Shutdown();
}

void SPrefabEditorViewport::Shutdown()
{
	if (ViewportClient.IsValid())
	{
		ViewportClient->Viewport = nullptr;
		ViewportClient.Reset();
	}
}

TSharedRef<FEditorViewportClient> SPrefabEditorViewport::MakeEditorViewportClient()
{
	ViewportClient = MakeShareable(new FPrefabEditorViewportClient(*PreviewScene.Get(), SharedThis(this)));
	return ViewportClient.ToSharedRef();
}
