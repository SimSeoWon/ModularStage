#include "MissionTask_SetCamera.h"
#include "ModularStage/Table/MissionTaskTable.h"

UMissionTask_SetCamera::UMissionTask_SetCamera()
{
}

void UMissionTask_SetCamera::SetData(const FMissionTaskInfo& InTaskInfo)
{
	Super::SetData(InTaskInfo);
	//TileIndex = InTaskInfo.TileIndex;
}

ETaskExecutorResult UMissionTask_SetCamera::DoAction()
{
	/*UAlphaGameInstance* gameInstance = UAlphaGameInstance::Get();
	if (false == IsValid(gameInstance))
		return ETaskExecutorResult::Fail;

	UWorld* world = gameInstance->GetWorld();
	if (false == IsValid(world))
		return ETaskExecutorResult::Fail;

	AAlphaLevelScriptActor* levelScriptActor = Cast<AAlphaLevelScriptActor>(world->GetLevelScriptActor());
	if (false == IsValid(levelScriptActor))
		return ETaskExecutorResult::Fail;

	if (false == levelScriptActor->SetCameraActor(TileIndex))
		return ETaskExecutorResult::Fail;*/

	return ETaskExecutorResult::Complete;
}

void UMissionTask_SetCamera::Clear()
{

	Super::Clear();
}