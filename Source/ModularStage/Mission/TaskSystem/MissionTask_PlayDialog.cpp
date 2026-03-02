#include "MissionTask_PlayDialog.h"
#include "ModularStage/Table/MissionTaskTable.h"

UMissionTask_PlayDialog::UMissionTask_PlayDialog()
{
}

void UMissionTask_PlayDialog::SetData(const FMissionTaskInfo& InTaskInfo)
{
	Super::SetData(InTaskInfo);
}

ETaskExecutorResult UMissionTask_PlayDialog::DoAction()
{
	//UAlphaManager_Dialog* manager  = UAlphaManager_Dialog::Get();
	//if (false == IsValid(manager))
	//	return ETaskExecutorResult::Fail;

	//if(false == manager->PlayDialog(1))
	//	return ETaskExecutorResult::Fail;
	//
	return ETaskExecutorResult::Complete;
}

void UMissionTask_PlayDialog::Clear()
{

	Super::Clear();
}