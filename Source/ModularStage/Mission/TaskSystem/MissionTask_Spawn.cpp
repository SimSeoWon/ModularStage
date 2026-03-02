#include "MissionTask_Spawn.h"
#include "ModularStage/Table/MissionTaskTable.h"
#include "ModularStage/Table/MissionTaskData.h"

UMissionTask_Spawn::UMissionTask_Spawn()
{

}

void UMissionTask_Spawn::SetData(const FMissionTaskInfo& InTaskInfo)
{
	Super::SetData(InTaskInfo);

	if (EInGameTaskType::SPAWN_MONSTER != InTaskInfo.Type)
		return;
	
	SpawnType = EInGameSpawnType::MONSTER;
	
	if (const FMissionTaskData_Monster* Data = InTaskInfo.TaskDetail.GetPtr<FMissionTaskData_Monster>())
	{
		TableID = Data->MonsterID;
		TileIndex = Data->TileIndex;
	}
}

ETaskExecutorResult UMissionTask_Spawn::DoAction()
{
	UE_LOG(LogTemp, Log, TEXT("UMissionTask_Spawn::DoAction - ActorID: %d, TableID: %d, TileIndex: %d, SpawnType: %d"), 
		ActorID, TableID, TileIndex, (int32)SpawnType);

	// TODO: Implement actual spawn logic using UEntityManager if available.
	return ETaskExecutorResult::Complete;
}


void UMissionTask_Spawn::Clear()
{

	Super::Clear();
}