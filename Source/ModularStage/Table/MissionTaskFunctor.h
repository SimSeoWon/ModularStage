// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
struct FMissionTaskInfo;

/**
 * 
 */
class FMissionTaskFunctor
{
public:
	inline FMissionTaskFunctor() {}
	virtual ~FMissionTaskFunctor() {}
	inline FMissionTaskFunctor(TSharedPtr<FMissionTaskInfo> inTaskInfo) : TaskInfo(inTaskInfo.Get()) {}
	inline FMissionTaskFunctor(FMissionTaskInfo* inTaskInfo) : TaskInfo(inTaskInfo) {}
	virtual void Clear();
	int32 GetValue(int32 index);
	void SetValue(int32 index, int32 inValue);

protected:
	FMissionTaskInfo* TaskInfo = nullptr;
};


class  FMissionTaskFunctor_Spawn : public FMissionTaskFunctor
{
public:

	inline FMissionTaskFunctor_Spawn() {}
	virtual ~FMissionTaskFunctor_Spawn() {}
	inline FMissionTaskFunctor_Spawn(TSharedPtr<FMissionTaskInfo> inTaskInfo) : FMissionTaskFunctor(inTaskInfo.Get()) {}
	inline FMissionTaskFunctor_Spawn(FMissionTaskInfo* inTaskInfo) : FMissionTaskFunctor(inTaskInfo) {}

	virtual void Clear() override;
	int32 GetActorID();
	int32 GetMonsterID();
	void SetActorID(int32 inActorID);
	void SetMonsterID(int32 inMonsterID);
};

class FMissionTaskFunctor_MarchOn : public FMissionTaskFunctor
{
public:
	inline FMissionTaskFunctor_MarchOn() {}
	virtual ~FMissionTaskFunctor_MarchOn() {}
	inline FMissionTaskFunctor_MarchOn(TSharedPtr<FMissionTaskInfo> inTaskInfo) : FMissionTaskFunctor(inTaskInfo.Get()) {}
	inline FMissionTaskFunctor_MarchOn(FMissionTaskInfo* inTaskInfo) : FMissionTaskFunctor(inTaskInfo) {}

	virtual void Clear() override;
	int32 GetActorID();
	void SetActorID(int32 inActorID);
};
