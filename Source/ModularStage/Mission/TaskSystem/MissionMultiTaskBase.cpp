#include "MissionMultiTaskBase.h"
#include "ModularStage/Table/MissionTaskTable.h"

UMissionMultiTaskBase::UMissionMultiTaskBase()
{
}

void UMissionMultiTaskBase::Clear()
{
	Super::Clear();

	Step = 0;

	if (IsValid(OccupiedTask))
	{
		OccupiedTask = nullptr;
	}

	for (auto iter : SubTaskList)
	{
		if (false == IsValid(iter))
			continue;

		iter->Clear();
	}

	SubTaskList.Reset();
}

void UMissionMultiTaskBase::SetData(const FMissionTaskInfo& InTaskInfo)
{
	Super::SetData(InTaskInfo);
	TaskInfo = InTaskInfo;
}

void UMissionMultiTaskBase::AddSubTask(TObjectPtr<UMissionTaskBase> inSubTask)
{
	TObjectPtr<UMissionTaskBase> subTask = inSubTask;
	if (false == IsValid(subTask))
		return;

	SubTaskList.Add(inSubTask);
}

ETaskExecutorResult UMissionMultiTaskBase::DoAction()
{
	if (SubTaskList.IsEmpty())
		return ETaskExecutorResult::Complete;

	if (false == IsValid(OccupiedTask))
	{
		if (false == SubTaskList.IsValidIndex(Step))
			return  ETaskExecutorResult::Complete;

		OccupiedTask = SubTaskList[Step];
	}

	ETaskExecutorResult result = OccupiedTask->DoAction();
	if (ETaskExecutorResult::Complete == result)
	{
		++Step;
		OccupiedTask = nullptr;
		result = ETaskExecutorResult::Progress;
	}

	return result;
}
