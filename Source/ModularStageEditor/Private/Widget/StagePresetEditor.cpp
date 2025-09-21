#include "StagePresetEditor.h"
#include "Editor.h"
#include "Kismet/GameplayStatics.h"

#include "LevelScriptActor_Generate.h"
#include "EditorUtilityWidgetComponents.h"


void UStagePresetEditor::NativeConstruct()
{
	Super::NativeConstruct();

	Btn_CreatePrefab->OnClicked.AddDynamic(this, &UStagePresetEditor::OnClicked_Prefab);

	Btn_CreatePrefab2->OnClicked.AddDynamic(this, &UStagePresetEditor::OnClicked_Prefab_Components);
}

ALevelScriptActor_Generate* UStagePresetEditor::GetLevelScriptActor()
{
	if (false == IsValid(GEditor))
		return nullptr;

	UWorld* world = GEditor->GetEditorWorldContext().World();
	if (false == IsValid(world))
		return nullptr;

	ALevelScriptActor* levelScript = world->GetLevelScriptActor();
	if (false == IsValid(levelScript))
		return nullptr;

	return  Cast<ALevelScriptActor_Generate>(levelScript);;
}

void UStagePresetEditor::OnClicked_Prefab() 
{
	ALevelScriptActor_Generate* generateLevel = GetLevelScriptActor();
	if (false == IsValid(generateLevel))
		return;

	FString targerFolder = Edit_FolderName->GetText().ToString();
	TArray<AActor*> arrActors = generateLevel->GetActorsInFolder(targerFolder);

	if(arrActors.IsEmpty())
		return;

	FString savedPath = Edit_PackagePath->GetText().ToString();
	generateLevel->CreatePrefabFromActors(arrActors, savedPath);
}

void UStagePresetEditor::OnClicked_Prefab_Components() 
{
	ALevelScriptActor_Generate* generateLevel = GetLevelScriptActor();
	if (false == IsValid(generateLevel))
		return;

	FString targerFolder = Edit_FolderName->GetText().ToString();
	TArray<AActor*> arrActors = generateLevel->GetActorsInFolder(targerFolder);

	if (arrActors.IsEmpty())
		return;

	FString savedPath = Edit_PackagePath->GetText().ToString();
	generateLevel->CreatePrefabFromCollectedComponents(arrActors, savedPath);
}