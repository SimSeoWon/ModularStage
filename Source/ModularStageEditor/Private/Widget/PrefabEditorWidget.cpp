#include "Widget/PrefabEditorWidget.h"
#include "Widget/SPrefabEditorViewport.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Input/SButton.h"
#include "Styling/AppStyle.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Engine/SimpleConstructionScript.h"
#include "Engine/SCS_Node.h"
#include "Misc/PackageName.h"
#include "Editor.h"
#include "FileHelpers.h"
#include "Framework/Notifications/NotificationManager.h"
#include "Widgets/Notifications/SNotificationList.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "ObjectTools.h" // [필수] 에셋 삭제 기능을 위해 추가
#include "Misc/PackageName.h"
#include "Misc/Paths.h" // FPaths 사용을 위해
#include "Editor.h"
#include "EditorUtilitySubsystem.h"
#include "Engine/Selection.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/Brush.h"
#include "GameFramework/WorldSettings.h"
#include "GameFramework/DefaultPhysicsVolume.h"
#include "EngineUtils.h"
#include "Engine/Blueprint.h"
#include "Engine/SimpleConstructionScript.h"
#include "Engine/SCS_Node.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "Mission/MissionPrefab.h"
#include "Mission/Component/MissionPropsComponent_Spawner.h"
#include "Mission/Component/MissionPropsComponent_Trigger.h"
#include "Mission/Component/MissionPropComponent.h"
#include "UObject/ObjectRedirector.h" // 리다이렉터 클래스 인식용
#include "PropertyEditorModule.h"
#include "AssetRegistry/IAssetRegistry.h"

#include "ContentBrowserModule.h"
#include "IContentBrowserSingleton.h"
#include "AssetToolsModule.h"
#include "Factories/BlueprintFactory.h"

#include "Widgets/Views/SListView.h"
#include "Widgets/Views/STableRow.h"
#include "Widgets/Layout/SSplitter.h"
#include "Widget/TaskListEditorWidget.h"

TSharedRef<SWidget> UPrefabEditorWidget::RebuildWidget()
{
	// Initialize the Advanced Preview Scene if it doesn't exist
	if (!PreviewScene.IsValid())
	{
		FAdvancedPreviewScene::ConstructionValues CVS;
		PreviewScene = MakeShareable(new FAdvancedPreviewScene(CVS));
		PreviewScene->SetFloorVisibility(true);
		PreviewScene->SetSkyBrightness(1.0f);
	}

	SAssignNew(ViewportWidget, SPrefabEditorViewport, PreviewScene);

	// Create Details View
	FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
	FDetailsViewArgs DetailsViewArgs;
	DetailsViewArgs.bUpdatesFromSelection = false;
	DetailsViewArgs.bLockable = false;
	DetailsViewArgs.bAllowSearch = true;
	DetailsViewArgs.NameAreaSettings = FDetailsViewArgs::ObjectsUseNameArea;
	DetailsViewArgs.NotifyHook = nullptr;
	DetailsView = PropertyEditorModule.CreateDetailView(DetailsViewArgs);

	// Create Hierarchy Tree View
	SAssignNew(HierarchyTreeView, STreeView<TSharedPtr<FPrefabEditorTreeItem>>)
		.TreeItemsSource(&HierarchySource)
		.OnGenerateRow_Lambda([this](TSharedPtr<FPrefabEditorTreeItem> InItem, const TSharedRef<STableViewBase>& OwnerTable) { return OnGenerateRowForHierarchy(InItem, OwnerTable); })
		.OnSelectionChanged_Lambda([this](TSharedPtr<FPrefabEditorTreeItem> InItem, ESelectInfo::Type SelectInfo) { OnHierarchySelectionChanged(InItem, SelectInfo); })
		.OnGetChildren_Lambda([this](TSharedPtr<FPrefabEditorTreeItem> InItem, TArray<TSharedPtr<FPrefabEditorTreeItem>>& OutChildren) { OnGetChildrenForHierarchy(InItem, OutChildren); })
		.SelectionMode(ESelectionMode::Single);

	// Bind selection change from Viewport
	ViewportWidget->GetOnSelectionChanged().BindLambda([this](UObject* SelectedObject)
		{
			if (DetailsView.IsValid())
			{
				DetailsView->SetObject(SelectedObject);
			}

			if (HierarchyTreeView.IsValid())
			{
				if (SelectedObject)
				{
					// Recursively find and select the item in the tree
					FindAndSelectHierarchyItem_Recursive(SelectedObject, HierarchySource);
				}
				else
				{
					HierarchyTreeView->ClearSelection();
				}
			}
		});

	TSharedRef<SWidget> ResultWidget = SNew(SBox)
		.Padding(4.0f)
		[
			SNew(SVerticalBox)
				// Top Toolbar
				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(0, 0, 0, 4)
				[
					SNew(SHorizontalBox)
						+ SHorizontalBox::Slot().AutoWidth()
						[
							SNew(SButton)
								.Text(FText::FromString("Import From Level"))
								.OnClicked_Lambda([this]() { ImportSelectedFromLevel(); return FReply::Handled(); })
						]
						+ SHorizontalBox::Slot().AutoWidth().Padding(4, 0, 0, 0)
						[
							SNew(SButton)
								.Text(FText::FromString("Load Prefab"))
								.OnClicked_Lambda([this]() { OnLoadClicked(); return FReply::Handled(); })
						]
						+ SHorizontalBox::Slot().AutoWidth().Padding(4, 0, 0, 0)
						[
							SNew(SButton)
								.Text(FText::FromString("Save To Prefab"))
								.ButtonColorAndOpacity(FLinearColor(0.2f, 0.6f, 0.2f, 1.0f))
								.OnClicked_Lambda([this]() { SavePrefab(); return FReply::Handled(); })
						]
						+ SHorizontalBox::Slot().AutoWidth().Padding(4, 0, 0, 0)
						[
							SNew(SButton)
								.Text(FText::FromString("Edit Mission Tasks"))
								.OnClicked_Lambda([this]() { OnEditMissionTasksClicked(); return FReply::Handled(); })
						]
						+ SHorizontalBox::Slot().AutoWidth().Padding(4, 0, 0, 0)
						[
							SNew(SButton)
								.Text(FText::FromString("Clear Scene"))
								.OnClicked_Lambda([this]() { ClearPreviewScene(); return FReply::Handled(); })
						]
				]
			// Content Area with 3-column Splitter
			+ SVerticalBox::Slot()
				.FillHeight(1.0f)
				[
					SNew(SSplitter)
						.Orientation(Orient_Horizontal)

						// Left: Component Palette
						+ SSplitter::Slot()
						.Value(0.2f)
						[
							SNew(SVerticalBox)
								+ SVerticalBox::Slot()
								.AutoHeight()
								.Padding(4)
								[
									SNew(STextBlock)
										.Text(FText::FromString("Add Mission Component"))
										.Font(FCoreStyle::GetDefaultFontStyle("Bold", 10))
								]
								+ SVerticalBox::Slot()
								.AutoHeight()
								.Padding(2)
								[
									SNew(SButton)
										.Text(FText::FromString("Add Spawner"))
										.OnClicked_Lambda([this]() { AddSpawner(); return FReply::Handled(); })
								]
								+ SVerticalBox::Slot()
								.AutoHeight()
								.Padding(2)
								[
									SNew(SButton)
										.Text(FText::FromString("Add Trigger"))
										.OnClicked_Lambda([this]() { AddTrigger(); return FReply::Handled(); })
								]
						]

					// Middle: Viewport
					+ SSplitter::Slot()
						.Value(0.5f)
						[
							ViewportWidget.ToSharedRef()
						]

						// Right: Hierarchy & Details
						+ SSplitter::Slot()
						.Value(0.3f)
						[
							SNew(SSplitter)
								.Orientation(Orient_Vertical)
								+ SSplitter::Slot() // Top: Hierarchy
								.Value(0.4f)
								[
									SNew(SVerticalBox)
										+ SVerticalBox::Slot().AutoHeight().Padding(4)
										[
											SNew(SHorizontalBox)
												+ SHorizontalBox::Slot().FillWidth(1.0f).Padding(0, 0, 4, 0)
												[
													SNew(SButton)
														.Text(FText::FromString("Preview All Spawners"))
														.OnClicked_Lambda([this]() { OnPreviewButtonClicked(); return FReply::Handled(); })
												]
												+ SHorizontalBox::Slot().FillWidth(1.0f)
												[
													SNew(SButton)
														.Text(FText::FromString("Clear All Previews"))
														.OnClicked_Lambda([this]() { OnClearPreviewButtonClicked(); return FReply::Handled(); })
												]
										]
									+ SVerticalBox::Slot().AutoHeight().Padding(4)
										[
											SNew(STextBlock)
												.Text(FText::FromString("Prefab Hierarchy"))
												.Font(FCoreStyle::GetDefaultFontStyle("Bold", 10))
										]
										+ SVerticalBox::Slot().FillHeight(1.0f)
										[
											SNew(SBorder)
												.BorderImage(FAppStyle::GetBrush("ToolPanel.GroupBorder"))
												[
													HierarchyTreeView.ToSharedRef()
												]
										]
								]
							+ SSplitter::Slot() // Bottom: Details
								.Value(0.6f)
								[
									DetailsView.ToSharedRef()
								]
						]
				]
		];

	RefreshHierarchy();

	return ResultWidget;
}

TSharedRef<ITableRow> UPrefabEditorWidget::OnGenerateRowForHierarchy(TSharedPtr<FPrefabEditorTreeItem> InItem, const TSharedRef<STableViewBase>& OwnerTable)
{
	FString Name = TEXT("Invalid");
	FSlateColor Color = FSlateColor::UseForeground();

	if (InItem.IsValid())
	{
		Name = InItem->DisplayName;

		if (InItem->Object.IsValid())
		{
			UObject* Obj = InItem->Object.Get();
			if (Obj->IsA<AActor>())
			{
				Color = FLinearColor(0.7f, 0.7f, 1.0f); // Actors are bluish
			}
			else if (Obj->IsA<UActorComponent>())
			{
				Color = FLinearColor(0.7f, 1.0f, 0.7f); // Components are greenish
			}
		}
		else
		{
			// Folder or Group
			Color = FLinearColor(1.0f, 0.8f, 0.4f); // Folders are yellowish
		}
	}

	return SNew(STableRow<TSharedPtr<FPrefabEditorTreeItem>>, OwnerTable)
		[
			SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.Padding(2, 0)
				[
					SNew(STextBlock)
						.Text(FText::FromString(Name))
						.ColorAndOpacity(Color)
				]
		];
}

void UPrefabEditorWidget::OnHierarchySelectionChanged(TSharedPtr<FPrefabEditorTreeItem> InItem, ESelectInfo::Type SelectInfo)
{
	if (InItem.IsValid() && DetailsView.IsValid())
	{
		UObject* SelectedObj = InItem->Object.Get();
		DetailsView->SetObject(SelectedObj);

		// Sync with Viewport if the selection came from a user interaction
		if (SelectInfo != ESelectInfo::Direct && ViewportWidget.IsValid())
		{
			ViewportWidget->SelectObject(SelectedObj, true);
		}
	}
}

void UPrefabEditorWidget::ImportSelectedFromLevel()
{
	if (!PreviewScene.IsValid()) return;

	USelection* SelectedActors = GEditor->GetSelectedActors();
	TArray<AActor*> Candidates;
	SelectedActors->GetSelectedObjects<AActor>(Candidates);

	AMissionPrefab* SeedActor = nullptr;
	TArray<AActor*> OtherActors;

	// 1. Validation: Find exactly one Seed
	for (AActor* Actor : Candidates)
	{
		if (Actor->IsA<AMissionPrefab>())
		{
			if (SeedActor != nullptr)
			{
				FNotificationInfo Info(FText::FromString("Multiple Mission Prefabs selected. Please select only one."));
				Info.ExpireDuration = 3.0f;
				FSlateNotificationManager::Get().AddNotification(Info);
				return;
			}
			SeedActor = Cast<AMissionPrefab>(Actor);
		}
		else // Collect all other selected actors
		{
			OtherActors.Add(Actor);
		}
	}

	if (SeedActor == nullptr)
	{
		FNotificationInfo Info(FText::FromString("No Mission Prefab selected. Please select one as a Seed."));
		Info.ExpireDuration = 3.0f;
		FSlateNotificationManager::Get().AddNotification(Info);
		return;
	}

	// 2. Clear existing preview
	ClearPreviewScene();

	UWorld* PreviewWorld = PreviewScene->GetWorld();
	FTransform SeedTransform = SeedActor->GetActorTransform();

	// 3. Clone Seed to (0,0,0)
	FActorSpawnParameters SpawnParams;
	AMissionPrefab* ClonedSeed = PreviewWorld->SpawnActor<AMissionPrefab>(SeedActor->GetClass(), FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	if (false == IsValid(ClonedSeed))
		return; // Failed to create seed

	ClonedSeed->UnregisterAllComponents(); // Re-add: Unregister components before copying
	UEngine::CopyPropertiesForUnrelatedObjects(SeedActor, ClonedSeed);
	ClonedSeed->RegisterAllComponents(); // Re-add: Re-register components after copying
	ClonedSeed->SetActorTransform(FTransform::Identity); // Ensure it's at origin

	for (AActor* Other : OtherActors)
	{
		// Calculate relative transform based on original SeedActor's transform
		FTransform RelativeTransform = Other->GetActorTransform().GetRelativeTransform(SeedTransform);

		// Get all components from the 'Other' actor
		TArray<UActorComponent*> ComponentsToCopy;
		Other->GetComponents(ComponentsToCopy);

		for (UActorComponent* OriginalComponent : ComponentsToCopy)
		{

			UActorComponent* NewComponent = NewObject<UActorComponent>(ClonedSeed, OriginalComponent->GetClass(), MakeUniqueObjectName(ClonedSeed, OriginalComponent->GetClass(), *OriginalComponent->GetName()));
			if (NewComponent)
			{
				// Copy properties from original component to new component
				UEngine::CopyPropertiesForUnrelatedObjects(OriginalComponent, NewComponent);

				// If it's a scene component, attach it and set its relative transform
				if (USceneComponent* NewSceneComponent = Cast<USceneComponent>(NewComponent))
				{
					NewSceneComponent->SetRelativeTransform(RelativeTransform); // Set component's relative transform
					NewSceneComponent->AttachToComponent(ClonedSeed->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
				}

				NewComponent->RegisterComponent();
				ClonedSeed->AddInstanceComponent(NewComponent); // Add to actor's instance components
			}
		}
	}

	RefreshHierarchy();

	FNotificationInfo SuccessInfo(FText::FromString("Import Successful!"));
	SuccessInfo.ExpireDuration = 2.0f;
	FSlateNotificationManager::Get().AddNotification(SuccessInfo);
}

void UPrefabEditorWidget::SavePrefab()
{
	// [FIX] Clear selection before performing destructive operations to prevent UI corruption.
	if (DetailsView.IsValid())
	{
		DetailsView->SetObject(nullptr);
	}
	if (HierarchyTreeView.IsValid())
	{
		HierarchyTreeView->ClearSelection();
	}
	if (ViewportWidget.IsValid())
	{
		ViewportWidget->ClearSelection();
	}

	if (!PreviewScene.IsValid()) return;

	// 1. Seed Actor 찾기
	UWorld* PreviewWorld = PreviewScene->GetWorld();
	AMissionPrefab* SeedActor = nullptr;
	for (TActorIterator<AMissionPrefab> It(PreviewWorld); It; ++It)
	{
		SeedActor = *It;
		break;
	}
	if (!SeedActor) return;

	// 2. 경로 설정 (기존 동일)
	FSaveAssetDialogConfig SaveAssetDialogConfig;
	SaveAssetDialogConfig.DialogTitleOverride = FText::FromString(TEXT("Save Mission Prefab"));
	SaveAssetDialogConfig.DefaultPath = TEXT("/Game/Blueprints");
	SaveAssetDialogConfig.DefaultAssetName = SeedActor->GetName();
	SaveAssetDialogConfig.ExistingAssetPolicy = ESaveAssetDialogExistingAssetPolicy::AllowButWarn;

	FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");
	FString SavePath = ContentBrowserModule.Get().CreateModalSaveAssetDialog(SaveAssetDialogConfig);

	if (SavePath.IsEmpty()) return;

	FString PackageName = FPackageName::ObjectPathToPackageName(SavePath);
	FString AssetName = FPaths::GetBaseFilename(SavePath);

	// 3. 패키지 로드 또는 생성
	UPackage* Package = CreatePackage(*PackageName);
	UBlueprint* Blueprint = nullptr;
	UObject* ExistingAsset = StaticFindObject(UObject::StaticClass(), Package, *AssetName);

	if (ExistingAsset)
	{
		Blueprint = Cast<UBlueprint>(ExistingAsset);
		if (!Blueprint) return;
	}
	else
	{
		FKismetEditorUtilities::FCreateBlueprintFromActorParams Params;
		Params.bOpenBlueprint = false;
		Params.ParentClassOverride = AMissionPrefab::StaticClass();

		Blueprint = FKismetEditorUtilities::CreateBlueprint(
			AMissionPrefab::StaticClass(),
			Package,
			FName(*AssetName),
			BPTYPE_Normal,
			UBlueprint::StaticClass(),
			UBlueprintGeneratedClass::StaticClass()
		);
	}

	if (!Blueprint) return;

	// 4. 수정 시작
	Blueprint->Modify();

	// A. CDO 복사 & InstanceComponents 비우기 (중복 방지)
	if (Blueprint->GeneratedClass)
	{
		AActor* CDO = Cast<AActor>(Blueprint->GeneratedClass->GetDefaultObject());
		if (CDO)
		{
			UEngine::CopyPropertiesForUnrelatedObjects(SeedActor, CDO);
			CDO->ClearInstanceComponents(true);
		}
	}

	// [FIX] 중복 또는 유효하지 않은 GUID 정리
	TSet<FGuid> KnownGuids;
	for (UActorComponent* Comp : SeedActor->GetComponents())
	{
		if (IMissionPropComponent* MissionComp = Cast<IMissionPropComponent>(Comp))
		{
			FGuid CurrentGuid = MissionComp->GetGuid();
			bool bIsDuplicate = false;
			KnownGuids.Add(CurrentGuid, &bIsDuplicate);

			if (!CurrentGuid.IsValid() || bIsDuplicate)
			{
				MissionComp->SetGuid(FGuid::NewGuid());
			}
		}
	}

	// B. SCS (컴포넌트 트리) 재구축
	USimpleConstructionScript* SCS = Blueprint->SimpleConstructionScript;
	if (SCS)
	{
		// 기존 노드 제거
		TArray<USCS_Node*> AllNodes = SCS->GetAllNodes();
		for (USCS_Node* Node : AllNodes)
		{
			SCS->RemoveNode(Node);
		}

		TArray<UActorComponent*> InstanceComponents;
		SeedActor->GetComponents(InstanceComponents);

		TMap<USceneComponent*, USCS_Node*> ComponentToNodeMap;

		// 1차 패스: 노드 생성
		for (UActorComponent* Comp : InstanceComponents)
		{
			// ★ [수정] Native(C++)만 아니면 모두 저장!
			// 기존에는 Instance만 체크해서 Load된 컴포넌트(SCS)가 누락되었습니다.
			if (!Comp || Comp->IsEditorOnly() || Comp->CreationMethod == EComponentCreationMethod::Native)
			{
				continue;
			}

			// 이제 CreationMethod가 Instance든 SimpleConstructionScript든 모두 저장됩니다.
			USCS_Node* NewNode = SCS->CreateNode(Comp->GetClass(), Comp->GetFName());
			SCS->AddNode(NewNode);

			if (NewNode->ComponentTemplate)
			{
				UEngine::CopyPropertiesForUnrelatedObjects(Comp, NewNode->ComponentTemplate);

				// Manually preserve the GUID to ensure it's not regenerated.
				if (IMissionPropComponent* LiveCompAsInterface = Cast<IMissionPropComponent>(Comp))
				{
					if (IMissionPropComponent* NewTemplateAsInterface = Cast<IMissionPropComponent>(NewNode->ComponentTemplate))
					{
						NewTemplateAsInterface->SetGuid(LiveCompAsInterface->GetGuid());
					}
				}
			}

			if (USceneComponent* SceneComp = Cast<USceneComponent>(Comp))
			{
				ComponentToNodeMap.Add(SceneComp, NewNode);
			}
		}

		// 2차 패스: 계층 구조 연결
		for (auto& Elem : ComponentToNodeMap)
		{
			USceneComponent* SceneComp = Elem.Key;
			USCS_Node* Node = Elem.Value;

			if (SceneComp && Node)
			{
				USceneComponent* ParentComp = SceneComp->GetAttachParent();

				// 부모가 있고, 부모 또한 이번에 저장되는 노드라면 Attach
				if (ParentComp && ComponentToNodeMap.Contains(ParentComp))
				{
					USCS_Node* ParentNode = ComponentToNodeMap[ParentComp];
					Node->SetParent(ParentNode);
				}
				// 부모가 C++ 루트 컴포넌트라면 자동으로 Root에 붙으므로 별도 처리 불필요
			}
		}
	}

	// 5. 컴파일 및 저장
	FKismetEditorUtilities::CompileBlueprint(Blueprint);

	TArray<UPackage*> PackagesToSave;
	PackagesToSave.Add(Package);
	FEditorFileUtils::PromptForCheckoutAndSave(PackagesToSave, false, false);

	if (GEditor)
	{
		TArray<UObject*> ObjectsToSync;
		ObjectsToSync.Add(Blueprint);
		GEditor->SyncBrowserToObjects(ObjectsToSync);
	}

	FNotificationInfo SuccessInfo(FText::FromString("Prefab Saved Successfully!"));
	SuccessInfo.ExpireDuration = 2.0f;
	FSlateNotificationManager::Get().AddNotification(SuccessInfo);

	// [UX Improvement] Automatically reload the prefab after saving
	if (IAssetRegistry* AssetRegistry = IAssetRegistry::Get())
	{
		FAssetData SavedAssetData = AssetRegistry->GetAssetByObjectPath(FSoftObjectPath(SavePath));
		if (SavedAssetData.IsValid())
		{
			LoadPrefab(SavedAssetData);
		}
	}
}

void UPrefabEditorWidget::ClearPreviewScene()
{
	if (!PreviewScene.IsValid()) return;

	UWorld* PreviewWorld = PreviewScene->GetWorld();
	for (TActorIterator<AActor> It(PreviewWorld); It; ++It)
	{
		TObjectPtr<AActor> Actor = *It;
		if (IsValid(Actor) && false == Actor->IsPendingKillPending())
		{
			PreviewWorld->DestroyActor(Actor);
		}
	}

	RefreshHierarchy();
}

void UPrefabEditorWidget::AddChildrenToTree(USceneComponent* ParentComponent, TSharedPtr<FPrefabEditorTreeItem> ParentItem)
{
	if (!ParentComponent || !ParentItem.IsValid()) return;

	TArray<USceneComponent*> Children;
	ParentComponent->GetChildrenComponents(false, Children);

	for (USceneComponent* Child : Children)
	{
		if (Child && !Child->IsVisualizationComponent())
		{
			TSharedPtr<FPrefabEditorTreeItem> ChildItem = MakeShared<FPrefabEditorTreeItem>(Child);
			ParentItem->Children.Add(ChildItem);
			AddChildrenToTree(Child, ChildItem); // Recurse
		}
	}
}

void UPrefabEditorWidget::OnGetChildrenForHierarchy(TSharedPtr<FPrefabEditorTreeItem> InItem, TArray<TSharedPtr<FPrefabEditorTreeItem>>& OutChildren)
{
	if (InItem.IsValid())
	{
		OutChildren = InItem->Children;
	}
}

bool UPrefabEditorWidget::FindAndSelectHierarchyItem_Recursive(UObject* ObjectToFind, const TArray<TSharedPtr<FPrefabEditorTreeItem>>& CurrentItems)
{
	if (!ObjectToFind || !HierarchyTreeView.IsValid())
	{
		return false;
	}

	for (const auto& Item : CurrentItems)
	{
		if (!Item.IsValid()) continue;

		// Check if the current item is the one we're looking for
		if (Item->Object.Get() == ObjectToFind)
		{
			HierarchyTreeView->SetSelection(Item, ESelectInfo::Direct);
			return true;
		}

		// If not, check its children recursively
		if (Item->Children.Num() > 0)
		{
			if (FindAndSelectHierarchyItem_Recursive(ObjectToFind, Item->Children))
			{
				// If the item was found in the children, expand the parent and return true
				HierarchyTreeView->SetItemExpansion(Item, true);
				return true;
			}
		}
	}

	return false;
}

AMissionPrefab* UPrefabEditorWidget::GetPrefabActor() const
{
	for (const auto& Item : HierarchySource)
	{
		if (Item.IsValid() && Item->Object.IsValid())
		{
			if (AMissionPrefab* Actor = Cast<AMissionPrefab>(Item->Object.Get()))
			{
				return Actor;
			}
		}
	}
	return nullptr;
}

void UPrefabEditorWidget::RefreshHierarchy()
{
	HierarchySource.Empty();
	if (!PreviewScene.IsValid()) return;

	UWorld* PreviewWorld = PreviewScene->GetWorld();

	AMissionPrefab* PrefabActor = nullptr;
	for (TActorIterator<AMissionPrefab> It(PreviewWorld); It; ++It)
	{
		PrefabActor = *It;
		break;
	}

	if (PrefabActor)
	{
		TSharedPtr<FPrefabEditorTreeItem> ActorRootItem = MakeShared<FPrefabEditorTreeItem>(PrefabActor);
		HierarchySource.Add(ActorRootItem);

		TArray<UActorComponent*> AllComponents;
		PrefabActor->GetComponents(AllComponents);

		for (UActorComponent* Comp : AllComponents)
		{
			if (Comp && !Comp->IsVisualizationComponent())
			{
				if (USceneComponent* SceneComp = Cast<USceneComponent>(Comp))
				{
					// Add root scene components to the actor's root
					if (SceneComp->GetAttachParent() == nullptr)
					{
						TSharedPtr<FPrefabEditorTreeItem> RootCompItem = MakeShared<FPrefabEditorTreeItem>(SceneComp);
						ActorRootItem->Children.Add(RootCompItem);
						AddChildrenToTree(SceneComp, RootCompItem);
					}
				}
				else
				{
					// Add non-scene components directly under the actor
					ActorRootItem->Children.Add(MakeShared<FPrefabEditorTreeItem>(Comp));
				}
			}
		}

		if (HierarchyTreeView.IsValid())
		{
			HierarchyTreeView->RequestTreeRefresh();
			HierarchyTreeView->SetItemExpansion(ActorRootItem, true);
		}
	}
	else
	{
		if (HierarchyTreeView.IsValid())
		{
			HierarchyTreeView->RequestTreeRefresh();
		}
	}

	// After refreshing the hierarchy, update the viewport client with the correct actor context.
	if (ViewportWidget.IsValid())
	{
		if (auto VpClient = ViewportWidget->GetPrefabEditorViewportClient())
		{
			VpClient->SetPrefabActor(GetPrefabActor());
		}
	}
}

void UPrefabEditorWidget::AddSpawner()
{
	if (!PreviewScene.IsValid()) return;
	UWorld* PreviewWorld = PreviewScene->GetWorld();

	// Find the single prefab actor in the scene
	AMissionPrefab* PrefabActor = nullptr;
	for (TActorIterator<AMissionPrefab> It(PreviewWorld); It; ++It)
	{
		PrefabActor = *It;
		break;
	}

	if (!PrefabActor)
	{
		FNotificationInfo Info(FText::FromString("No Prefab Actor in scene to add component to. Please Load or Import a prefab first."));
		FSlateNotificationManager::Get().AddNotification(Info);
		return;
	}

	// Add the component directly to the prefab actor
	UMissionPropsComponent_Spawner* NewSpawnerComponent = NewObject<UMissionPropsComponent_Spawner>(PrefabActor, UMissionPropsComponent_Spawner::StaticClass(), FName(TEXT("NewSpawner")));
	if (NewSpawnerComponent)
	{
		NewSpawnerComponent->SetResourcesPath(TEXT(""));
		NewSpawnerComponent->AttachToComponent(PrefabActor->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		NewSpawnerComponent->RegisterComponent();
		PrefabActor->AddInstanceComponent(NewSpawnerComponent); // Make sure the actor knows about its new component
		RefreshHierarchy();
	}
}

void UPrefabEditorWidget::AddTrigger()
{
	if (!PreviewScene.IsValid()) return;
	UWorld* PreviewWorld = PreviewScene->GetWorld();

	// Find the single prefab actor in the scene
	AMissionPrefab* PrefabActor = nullptr;
	for (TActorIterator<AMissionPrefab> It(PreviewWorld); It; ++It)
	{
		PrefabActor = *It;
		break;
	}

	if (!PrefabActor)
	{
		FNotificationInfo Info(FText::FromString("No Prefab Actor in scene to add component to. Please Load or Import a prefab first."));
		FSlateNotificationManager::Get().AddNotification(Info);
		return;
	}

	// Add the component directly to the prefab actor
	UMissionPropsComponent_Trigger* NewTriggerComponent = NewObject<UMissionPropsComponent_Trigger>(PrefabActor, UMissionPropsComponent_Trigger::StaticClass(), FName(TEXT("NewTrigger")));
	if (NewTriggerComponent)
	{
		NewTriggerComponent->AttachToComponent(PrefabActor->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		NewTriggerComponent->RegisterComponent();
		PrefabActor->AddInstanceComponent(NewTriggerComponent); // Make sure the actor knows about its new component
		RefreshHierarchy();
	}
}

void UPrefabEditorWidget::OnLoadClicked()
{
	FContentBrowserModule& ContentBrowserModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>("ContentBrowser");

	FOpenAssetDialogConfig OpenAssetDialogConfig;
	OpenAssetDialogConfig.DialogTitleOverride = FText::FromString("Load Mission Prefab");
	OpenAssetDialogConfig.DefaultPath = TEXT("/Game/Blueprints/Mission/Prefab");
	OpenAssetDialogConfig.AssetClassNames.Add(UBlueprint::StaticClass()->GetClassPathName());

	TArray<FAssetData> SelectedAssets = ContentBrowserModule.Get().CreateModalOpenAssetDialog(OpenAssetDialogConfig);

	if (SelectedAssets.Num() > 0)
	{
		const FAssetData& AssetData = SelectedAssets[0];
		UObject* Asset = AssetData.GetAsset();
		if (UBlueprint* Blueprint = Cast<UBlueprint>(Asset))
		{
			if (Blueprint->GeneratedClass && Blueprint->GeneratedClass->IsChildOf<AMissionPrefab>())
			{
				LoadPrefab(AssetData);
			}
			else
			{
				FNotificationInfo Info(FText::FromString(TEXT("Selected Blueprint must inherit from AMissionPrefab.")));
				Info.ExpireDuration = 3.0f;
				FSlateNotificationManager::Get().AddNotification(Info);
			}
		}
		else
		{
			FNotificationInfo Info(FText::FromString(TEXT("Failed to load selected asset or it is not a Blueprint.")));
			Info.ExpireDuration = 3.0f;
			FSlateNotificationManager::Get().AddNotification(Info);
		}
	}
}

void UPrefabEditorWidget::LoadPrefab(const FAssetData& AssetData)
{
	UBlueprint* Blueprint = Cast<UBlueprint>(AssetData.GetAsset());
	// Validation already happened in OnLoadClicked, but a quick check won't hurt.
	if (!Blueprint || !Blueprint->GeneratedClass || !Blueprint->GeneratedClass->IsChildOf(AMissionPrefab::StaticClass()))
	{
		FNotificationInfo Info(FText::FromString("Internal Error: Invalid Blueprint passed to LoadPrefab."));
		FSlateNotificationManager::Get().AddNotification(Info);
		return;
	}

	// 1. Clear scene
	ClearPreviewScene();

	UWorld* PreviewWorld = PreviewScene->GetWorld();

	// 2. Spawn the Seed Actor (the Prefab itself)
	FActorSpawnParameters SpawnParams;
	UObject* CDO = Blueprint->GeneratedClass->GetDefaultObject(); // Get CDO to copy properties from

	AMissionPrefab* ClonedSeed = PreviewWorld->SpawnActor<AMissionPrefab>(Blueprint->GeneratedClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	if (ClonedSeed)
	{
		ClonedSeed->SetActorLabel(AssetData.AssetName.ToString()); // Use AssetName as label
	}

	RefreshHierarchy();

	FNotificationInfo SuccessInfo(FText::FromString(FString::Printf(TEXT("Loaded Prefab: %s"), *AssetData.AssetName.ToString())));
	SuccessInfo.ExpireDuration = 3.0f;
	FSlateNotificationManager::Get().AddNotification(SuccessInfo);
}

void UPrefabEditorWidget::BeginDestroy()
{
	Super::BeginDestroy();
}

void UPrefabEditorWidget::OnEditorExit_Implement()
{
	// Reset the viewport widget first, as it holds a reference to the scene via the viewport client.
	if (ViewportWidget.IsValid())
	{
		ViewportWidget->Shutdown();
		ViewportWidget.Reset();
	}

	if (PreviewScene.IsValid())
	{
		PreviewScene.Reset();
	}
}

void UPrefabEditorWidget::OnPreviewButtonClicked()
{
	// Clear any existing preview actors first
	OnClearPreviewButtonClicked();

	AMissionPrefab* PrefabActor = GetPrefabActor();
	if (!PrefabActor)
	{
		FNotificationInfo Info(FText::FromString("No Prefab Actor loaded. Please load a prefab first."));
		Info.ExpireDuration = 3.0f;
		FSlateNotificationManager::Get().AddNotification(Info);
		return;
	}

	TArray<UMissionPropsComponent_Spawner*> AllSpawners;
	PrefabActor->GetComponents<UMissionPropsComponent_Spawner>(AllSpawners);

	if (AllSpawners.Num() == 0)
	{
		FNotificationInfo Info(FText::FromString("No Spawner components found in the loaded prefab."));
		Info.ExpireDuration = 3.0f;
		FSlateNotificationManager::Get().AddNotification(Info);
		return;
	}

	UWorld* World = PreviewScene->GetWorld();
	bool bFoundAnyValidSpawner = false;

	for (UMissionPropsComponent_Spawner* Spawner : AllSpawners)
	{
		if (!Spawner) continue;

		const FString& AssetPath = Spawner->ResourcesPath;
		if (AssetPath.IsEmpty())
		{
			continue; // Skip spawners with no path
		}

		UClass* ClassToSpawn = nullptr;
		UObject* LoadedObject = StaticLoadObject(UObject::StaticClass(), nullptr, *AssetPath);

		if (UBlueprint* Blueprint = Cast<UBlueprint>(LoadedObject))
		{
			ClassToSpawn = Blueprint->GeneratedClass;
		}
		else if (UClass* LoadedClass = Cast<UClass>(LoadedObject))
		{
			ClassToSpawn = LoadedClass;
		}

		if (!ClassToSpawn || !ClassToSpawn->IsChildOf<AActor>())
		{
			// Maybe notify per-spawner error? For now, just skip.
			continue;
		}

		const FTransform ComponentTransform = Spawner->GetComponentTransform();
		const TArray<int32>& SpawnTileIndices = Spawner->SpawnTileIndices;

		if (SpawnTileIndices.Num() == 0)
		{
			continue; // Skip spawners with no indices
		}

		// [단일화] 데이터 기반 자신의 타일 위치 (CenterLocation)
		const FVector MyTileLoc = PrefabActor->GetTileLocation(Spawner->GetHexTileIndex());

		bFoundAnyValidSpawner = true;
		FActorSpawnParameters SpawnParams;
		SpawnParams.ObjectFlags = RF_Transient; // Ensure preview actors are not saved

		for (int32 TileIdx : SpawnTileIndices)
		{
			// [단일화] 대상 타일 중심 - 내 타일 중심 = 정확한 상대 오프셋
			FVector RelativeOffset = PrefabActor->GetTileLocation(TileIdx) - MyTileLoc;
			FTransform SpawnTransform(ComponentTransform.TransformPosition(RelativeOffset));
			AActor* NewActor = World->SpawnActor<AActor>(ClassToSpawn, SpawnTransform, SpawnParams);
			if (NewActor)
			{
				PreviewActors.Add(NewActor);
			}
		}
	}

	if (!bFoundAnyValidSpawner)
	{
		FNotificationInfo Info(FText::FromString("No spawners with valid asset paths and spawn points were found."));
		Info.ExpireDuration = 3.0f;
		FSlateNotificationManager::Get().AddNotification(Info);
	}
}

void UPrefabEditorWidget::OnClearPreviewButtonClicked()
{
	UWorld* World = PreviewScene->GetWorld();
	for (TObjectPtr<AActor> Actor : PreviewActors)
	{
		if (Actor && IsValid(Actor))
		{
			World->DestroyActor(Actor);
		}
	}
	PreviewActors.Empty();
}

UMissionPropsComponent_Spawner* UPrefabEditorWidget::GetSelectedSpawnerComponent() const
{
	if (HierarchyTreeView.IsValid() && HierarchyTreeView->GetNumItemsSelected() > 0)
	{
		TArray<TSharedPtr<FPrefabEditorTreeItem>> SelectedItems;
		HierarchyTreeView->GetSelectedItems(SelectedItems);

		if (SelectedItems.Num() > 0 && SelectedItems[0].IsValid() && SelectedItems[0]->Object.IsValid())
		{
			return Cast<UMissionPropsComponent_Spawner>(SelectedItems[0]->Object.Get());
		}
	}
	return nullptr;
}

void UPrefabEditorWidget::OnEditMissionTasksClicked()
{
	AMissionPrefab* prefabActor = GetPrefabActor();
	if (!prefabActor)
	{
		FNotificationInfo Info(FText::FromString("Please load a Prefab before editing tasks."));
		Info.ExpireDuration = 3.0f;
		FSlateNotificationManager::Get().AddNotification(Info);
		return;
	}

	UTaskListEditorWidget* taskEditor = UEditorWidgetBase::ShowEditorUtilityWidget<UTaskListEditorWidget>();
	if (IsValid(taskEditor))
	{
		taskEditor->SetTargetActor(prefabActor);
	}
}
