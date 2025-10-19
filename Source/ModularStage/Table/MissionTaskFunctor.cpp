// Fill out your copyright notice in the Description page of Project Settings.


#include "MissionTaskFunctor.h"
#include "MissionTaskTable.h"

void FMissionTaskFunctor::Clear() 
{
	if (nullptr == TaskInfo)
		return;

	TaskInfo->IntegerParamList.Reset();
}

int32 FMissionTaskFunctor::GetValue(int32 index) 
{
	if (nullptr == TaskInfo)
		return -1;

	if (false == TaskInfo->IntegerParamList.IsValidIndex(index))
		return -1;

	return TaskInfo->IntegerParamList[index];
}

void FMissionTaskFunctor::SetValue(int32 index, int32 inValue) 
{
	if (nullptr == TaskInfo)
		return;

	if (false == TaskInfo->IntegerParamList.IsValidIndex(index))
		return;

	TaskInfo->IntegerParamList[index] = inValue;
}

void FMissionTaskFunctor_Spawn::Clear()
{
	FMissionTaskFunctor::Clear();

	if (nullptr == TaskInfo)
		return;

	TaskInfo->IntegerParamList.Reserve(2);

	TaskInfo->IntegerParamList.Emplace(0); // ActorID
	TaskInfo->IntegerParamList.Emplace(0); // MonsterID
}

int32 FMissionTaskFunctor_Spawn::GetActorID()
{
	return GetValue(0);
}

int32 FMissionTaskFunctor_Spawn::GetMonsterID()
{
	return GetValue(1);
}

void  FMissionTaskFunctor_Spawn::SetActorID(int32 inActorID)
{
	SetValue(0, inActorID);
}

void  FMissionTaskFunctor_Spawn::SetMonsterID(int32 inMonsterID) 
{
	SetValue(1, inMonsterID);
}

void FMissionTaskFunctor_MarchOn::Clear() 
{
	FMissionTaskFunctor::Clear();

	if (nullptr == TaskInfo)
		return;

	TaskInfo->IntegerParamList.Reserve(1);
	TaskInfo->IntegerParamList.Emplace(0); // ActorID
}

int32 FMissionTaskFunctor_MarchOn::GetActorID() 
{
	return GetValue(0);
}

void FMissionTaskFunctor_MarchOn::SetActorID(int32 inActorID) 
{
	SetValue(0, inActorID);
}