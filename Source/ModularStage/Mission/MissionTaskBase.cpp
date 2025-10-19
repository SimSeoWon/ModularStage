//Fill out your copyright notice in the Description page of Project Settings.


#include "MissionTaskBase.h"



FMissionTaskBase::FMissionTaskBase()
{
	REGIST_PEDIGREE(FMissionTaskBase);

}

FMissionMultiTaskBase::FMissionMultiTaskBase()
	: FMissionTaskBase()
{
	REGIST_PEDIGREE(FMissionMultiTaskBase);
}

void FMissionMultiTaskBase::Clear()
{
	FMissionTaskBase::Clear();

	Step = 0;

	if (OccupiedTask.IsValid())
	{
		OccupiedTask = nullptr;
	}

	for (auto iter : SubTaskList)
	{
		if (false == iter.IsValid())
			continue;

		iter->Clear(); //�����Ѵ�.
	}
}

void FMissionMultiTaskBase::SetData(TSharedPtr<FMissionTaskInfo> inTaskInfo)
{
	FMissionTaskBase::SetData(inTaskInfo);
	TaskInfo = inTaskInfo;
	// �½�ũ ����, Ÿ��Ʋ, ����, ���ǵ��� ������...
	//Step = TaskInfo->Step;
	//TaskType = TaskInfo->TaskType;
	//Title = TaskInfo->Title;
	//Desc = TaskInfo->Desc;
}

void FMissionMultiTaskBase::AddSubTask(TSharedRef<FMissionTaskBase> inSubTask)
{
	TSharedPtr<FMissionTaskBase> subTask = inSubTask;
	if (false == subTask.IsValid())
		return;

	SubTaskList.Add(inSubTask);
}

ETaskExecutorResult FMissionMultiTaskBase::DoAction()
{
	/**��ϵ� ���� �½�ũ�� ����*/
	if (SubTaskList.IsEmpty())
		return ETaskExecutorResult::Complete;

	if (false == OccupiedTask.IsValid())
	{
		/**���� ���� �½�ũ�� ����*/
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

FMissionTask_MarchOn::FMissionTask_MarchOn()
	:FMissionTaskBase()
{
	REGIST_PEDIGREE(FMissionTask_MarchOn);
}

void FMissionTask_MarchOn::SetData(TSharedPtr<FMissionTaskInfo> inTaskInfo)
{
	FMissionTaskBase::SetData(inTaskInfo);
	
	ActorID = 0; // ���� ���̵�
	TileIndex = -1;		// �������� Ÿ�� �ε���
	//TileIndex = inTaskInfo->TileIndex;

	//if (inTaskInfo->TaskType == EInGameTaskType::MARCH_ON)
	//{
	//	FMissionTaskFunctor_MarchOn functor(inTaskInfo);
	//	ActorID = functor.GetActorID();
	//}
}

ETaskExecutorResult FMissionTask_MarchOn::DoAction()
{
	//UEntityManager* manager = UEntityManager::Get();
	//if (false == IsValid(manager))
	//	return ETaskExecutorResult::Fail;

	//EntityId entityId(ActorID);
	//Entity* entity = manager->FindEntity(entityId);
	//if (nullptr == entity)
	//	return ETaskExecutorResult::Fail;

	////entity ���� �̵��� ����Ѵ�.
	////�ڽ��� �Ͽ� Character�� entity�� ��ϵ� ������ ��������.
	//entity->SetDestination(TileIndex);
	return  ETaskExecutorResult::Complete;
}


FMissionTask_Spawn::FMissionTask_Spawn()
	:FMissionTaskBase()
{
	REGIST_PEDIGREE(FMissionTask_Spawn);
}

void FMissionTask_Spawn::SetData(TSharedPtr<FMissionTaskInfo> inTaskInfo)
{
	// �۵��� Ÿ�� ��ġ ��������
	FMissionTaskBase::SetData(inTaskInfo);

	/*TileIndex = inTaskInfo->TileIndex;
	SpawnType = EInGameSpawnType::NONE;
	ActorID = 0;
	TableID = 0;

	if (inTaskInfo->TaskType == EInGameTaskType::SPAWN_MONSTER)
	{
		FMissionTaskFunctor_Spawn functor(inTaskInfo);

		SpawnType = EInGameSpawnType::MONSTER;
		ActorID = functor.GetActorID();
		TableID = functor.GetMonsterID();
	}*/
}

ETaskExecutorResult FMissionTask_Spawn::DoAction()
{
	//UEntityManager* manager = UEntityManager::Get();
	//if (false == IsValid(manager))
	//	return ETaskExecutorResult::Fail;

	//TSharedPtr<Entity> entity = manager->CreateEntity(TileIndex, ActorID, TableID);
	//if (false == entity.IsValid())
	//	return ETaskExecutorResult::Fail;

	////Todo ���߿� TileIndex�� �������� ȭ�鿡 ���̴� �ֵ鸸 ������Ű��.
	//manager->CreateActor(entity);


	return ETaskExecutorResult::Complete;
}

FMissionTask_SetCamera::FMissionTask_SetCamera()
	:FMissionTaskBase()
{
	REGIST_PEDIGREE(FMissionTask_SetCamera);
}

void FMissionTask_SetCamera::SetData(TSharedPtr<FMissionTaskInfo> inTaskInfo)
{
	FMissionTaskBase::SetData(inTaskInfo);
	//TileIndex = inTaskInfo->TileIndex;
}

ETaskExecutorResult FMissionTask_SetCamera::DoAction()
{
	//Todo ������ ��ǥ��(TileIndex)�� ī�޶� �����δ�.
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

FMissionTask_PlayDialog::FMissionTask_PlayDialog()
	:FMissionTaskBase()
{
	REGIST_PEDIGREE(FMissionTask_PlayDialog);
}

void FMissionTask_PlayDialog::SetData(TSharedPtr<FMissionTaskInfo> inTaskInfo)
{
	FMissionTaskBase::SetData(inTaskInfo);
}

ETaskExecutorResult FMissionTask_PlayDialog::DoAction()
{
	//UAlphaManager_Dialog* manager  = UAlphaManager_Dialog::Get();
	//if (false == IsValid(manager))
	//	return ETaskExecutorResult::Fail;

	//if(false == manager->PlayDialog(1))
	//	return ETaskExecutorResult::Fail;
	//
	//Todo �ӽ÷� ��� �����Ű��! ���� ���̾�α� ������� ETaskExecutorResult::Progress; �� �����ؾ��Ѵ�.
	return ETaskExecutorResult::Complete;
}