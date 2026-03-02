#include "MissionTask_ObjectSpawn.h"
#include "ModularStage/Table/MissionTaskTable.h"
#include "ModularStage/Table/MissionTaskData.h"

UMissionTask_ObjectSpawn::UMissionTask_ObjectSpawn()
{
}

void UMissionTask_ObjectSpawn::SetData(const FMissionTaskInfo& InTaskInfo)
{
	Super::SetData(InTaskInfo);


	if (EInGameTaskType::SPAWN_OBJECT != InTaskInfo.Type)
		return;

	SpawnType = EInGameSpawnType::OBJECT;

	if (const FMissionTaskData_Object* Data = InTaskInfo.TaskDetail.GetPtr<FMissionTaskData_Object>())
	{
		ActorID = Data->ActorID;

		TileIndex = Data->TileIndex;
	}
}

ETaskExecutorResult UMissionTask_ObjectSpawn::DoAction()
{
	UE_LOG(LogTemp, Log, TEXT("UMissionTask_ObjectSpawn::DoAction - ActorID: %d, TableID: %d, TileIndex: %d, SpawnType: %d"),
		ActorID, TableID, TileIndex, (int32)SpawnType);

	// TODO: Implement actual spawn logic using UEntityManager if available.
	return ETaskExecutorResult::Complete;
}


void UMissionTask_ObjectSpawn::Clear()
{

	Super::Clear();
}