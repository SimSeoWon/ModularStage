#include "MissionTaskExecutor.h"
#include "MissionTaskBase.h"

FMissionTaskExecutor::FMissionTaskExecutor()
	: bIsInitialized(false)
{
}

void FMissionTaskExecutor::Initialize(const FDataTableRowHandle& InMissionData)
{
	MissionData = InMissionData;
	bIsInitialized = true;
	// Initialize other states
}

void FMissionTaskExecutor::Tick(float DeltaTime)
{
	if (!bIsInitialized)
	{
		return;
	}
	// Core mission execution logic here
}
#pragma region Create Task Function
bool FMissionTaskExecutor::CreateTask(TSharedRef<FMissionTaskInfo> inTaskInfo)
{
	return true;
}

bool FMissionTaskExecutor::OnCreateTask_SPAWN(TSharedRef<FMissionTaskInfo> inTaskInfo)
{
	return true;
}

bool FMissionTaskExecutor::OnCreateTask_SPAWN_MONSTER(TSharedRef<FMissionTaskInfo> inTaskInfo)
{
	return true;
}

bool FMissionTaskExecutor::OnCreateTask_MARCH_ON(TSharedRef<FMissionTaskInfo> inTaskInfo)
{
	return true;
}

bool FMissionTaskExecutor::OnCreateTask_MISSION(TSharedRef<FMissionTaskInfo> inTaskInfo)
{
	return true;
}
#pragma endregion