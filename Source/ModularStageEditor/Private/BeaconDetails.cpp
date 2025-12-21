#include "BeaconDetails.h"
#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "DetailWidgetRow.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Text/STextBlock.h"
#include "UObject/UnrealType.h"
#include "GameFramework/Actor.h"

#include "Widget/EditorWidget_AssetSelector.h"
#include "Editor.h"

#include "Mission/Beacon.h"

#include "Subsystems/AssetEditorSubsystem.h"
#include "BlueprintEditor.h"
#include "AdvancedPreviewScene.h"
#include "PropertyEditorModule.h" // For IPropertyHandle
#include "IDetailPropertyRow.h" // For IDetailLayoutBuilder::GetProperty


TSharedRef<IDetailCustomization> FBeaconDetails::MakeInstance()
{
	return MakeShareable(new FBeaconDetails);
}

FBeaconDetails::~FBeaconDetails()
{
	if (PreviewActorPtr.IsValid() && PreviewActorPtr->GetWorld())
	{
		PreviewActorPtr->GetWorld()->DestroyActor(PreviewActorPtr.Get());
	}
}

void FBeaconDetails::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	TArray<TWeakObjectPtr<UObject>> SelectedObjects;
	DetailBuilder.GetObjectsBeingCustomized(SelectedObjects);

	if (SelectedObjects.Num() != 1)
	{
		return;
	}

	TWeakObjectPtr<ABeacon> Beacon = Cast<ABeacon>(SelectedObjects[0].Get());
	if (!Beacon.IsValid())
	{
		return;
	}

	// Get property handle for PrefabAssetPath
	PrefabAssetPathHandle = DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(ABeacon, PrefabAssetPath));
	if (PrefabAssetPathHandle.IsValid())
	{
		// Bind to property change delegate to clear the preview
		PrefabAssetPathHandle->SetOnPropertyValueChanged(FSimpleDelegate::CreateSP(this, &FBeaconDetails::ClearExistingPreview, Beacon));
	}

	IDetailCategoryBuilder& BeaconCategory = DetailBuilder.EditCategory("Beacon|Editor", FText::GetEmpty(), ECategoryPriority::Important);

	BeaconCategory.AddCustomRow(FText::GetEmpty())
	.NameContent()
	[
		SNullWidget::NullWidget
	]
	.ValueContent()
	[
		SNew(SHorizontalBox)
		+SHorizontalBox::Slot()
		.Padding(2.f)
		[
			SNew(SButton)
			.Text(FText::FromString(TEXT("Import Prefab")))
			.OnClicked_Lambda([this, Beacon]()
			{
				if (UEditorWidget_AssetSelector* EditorWidget = UEditorWidgetBase::ShowEditorUtilityWidget<UEditorWidget_AssetSelector>())
				{
					EditorWidget->GetOnAcceptAssetPath().BindLambda([this, Beacon, EditorWidget](FString AssetPath)
					{
						if (Beacon.IsValid())
						{
							ABeacon* BeaconActor = Beacon.Get();
							
							if (BeaconActor->GetWorld())
							{
								BeaconActor->OnSelectAssetPath(AssetPath);
							}
							else
							{
								UE_LOG(LogTemp, Warning, TEXT("Modifying CDO: Setting PrefabAssetPath directly for %s"), *BeaconActor->GetName());

								if (FProperty* PrefabAssetPathProperty = ABeacon::StaticClass()->FindPropertyByName(GET_MEMBER_NAME_CHECKED(ABeacon, PrefabAssetPath)))
								{
									BeaconActor->PreEditChange(PrefabAssetPathProperty);
									BeaconActor->PrefabAssetPath = AssetPath;
									FPropertyChangedEvent PropertyChangedEvent(PrefabAssetPathProperty, EPropertyChangeType::ValueSet);
									BeaconActor->PostEditChangeProperty(PropertyChangedEvent);
								}
							}
							// Automatically clear the old preview when path is changed
							ClearExistingPreview(Beacon);
						}

					});
				}
				return FReply::Handled();
			})
		]
		+SHorizontalBox::Slot()
		.Padding(2.f)
		[
			SNew(SButton)
			.Text(FText::FromString(TEXT("Preview")))
			.OnClicked_Lambda([this, Beacon]()
			{
				if(Beacon.IsValid())
				{
					UpdatePreviewInternal(Beacon.Get());
				}
				return FReply::Handled();
			})
		]
	];
}


void FBeaconDetails::UpdatePreviewInternal(ABeacon* BeaconActor)
{
	if (!BeaconActor)
	{
		return;
	}

	if (BeaconActor->GetWorld() && BeaconActor->GetWorld()->IsEditorWorld())
	{
		// Level instance preview
		BeaconActor->UpdatePreview();
	}
	else // This is a CDO, handle blueprint editor preview
	{
		// 1. Destroy old preview actor
		if (PreviewActorPtr.IsValid())
		{
			if(UWorld* ActorWorld = PreviewActorPtr->GetWorld())
			{
				ActorWorld->DestroyActor(PreviewActorPtr.Get());
			}
			PreviewActorPtr.Reset();
		}

		// 2. Load the class we want to spawn
		TSubclassOf<AActor> ChildClass = nullptr;
		if (!BeaconActor->PrefabAssetPath.IsEmpty())
		{
			FSoftObjectPath AssetPath(BeaconActor->PrefabAssetPath);
			if (UBlueprint* BlueprintAsset = Cast<UBlueprint>(AssetPath.TryLoad()))
			{
				ChildClass = Cast<UClass>(BlueprintAsset->GeneratedClass);
			}
		}

		if (!ChildClass)
		{
			return; // No class to preview, and old preview was destroyed
		}

		// 3. Find the preview world
		if (UBlueprint* BlueprintToEdit = UBlueprint::GetBlueprintFromClass(BeaconActor->GetClass()))
		{
			if (UAssetEditorSubsystem* AssetEditorSubsystem = GEditor->GetEditorSubsystem<UAssetEditorSubsystem>())
			{
				if (IAssetEditorInstance* AssetEditor = AssetEditorSubsystem->FindEditorForAsset(BlueprintToEdit, false))
				{
					if (FBlueprintEditor* BlueprintEditor = static_cast<FBlueprintEditor*>(AssetEditor))
					{
						if (FPreviewScene* PreviewScene = BlueprintEditor->GetPreviewScene())
						{
							if (UWorld* PreviewWorld = PreviewScene->GetWorld())
							{
								// 4. Spawn the new actor
								FActorSpawnParameters SpawnParams;
								SpawnParams.bNoFail = true;
								SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
								
								AActor* NewPreviewActor = PreviewWorld->SpawnActor<AActor>(ChildClass, BeaconActor->RelativeTransform, SpawnParams);
								PreviewActorPtr = NewPreviewActor;
							}
						}
					}
				}
			}
		}
	}
}

void FBeaconDetails::ClearExistingPreview(TWeakObjectPtr<ABeacon> Beacon)
{
	if (!Beacon.IsValid())
	{
		return;
	}
	
	ABeacon* BeaconActor = Beacon.Get();
	
	if (BeaconActor->GetWorld() && BeaconActor->GetWorld()->IsEditorWorld())
	{
		// Level instance preview
		BeaconActor->ClearPreview();
	}
	else // This is a CDO, handle blueprint editor preview
	{
		// Destroy old preview actor
		if (PreviewActorPtr.IsValid())
		{
			if(UWorld* ActorWorld = PreviewActorPtr->GetWorld())
			{
				ActorWorld->DestroyActor(PreviewActorPtr.Get());
			}
			PreviewActorPtr.Reset();
		}
	}
}
