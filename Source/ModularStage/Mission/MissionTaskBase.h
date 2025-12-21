// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ModularStage/Table/TableEnum.h"
struct FMissionTaskInfo;

#define REGIST_PEDIGREE(inClass) \
	{ \
		Name = #inClass; \
		Pedigree.Add(#inClass); \
	}

#define TRYCAST_TASK(inClass, inTask, outResult) \
	{ \
		if(inTask.IsValid()) \
		{\
			if(inTask->Pedigree.Contains(#inClass)) \
			{	\
				outResult = StaticCastSharedPtr<inClass>(inTask); \
			}	\
		}\
	}

class FMissionTaskBase
{
public:
	FMissionTaskBase();
	virtual ~FMissionTaskBase() {}
	virtual void Clear() {}
	virtual void SetData(TSharedPtr<FMissionTaskInfo> inTaskInfo)  {}
	virtual ETaskExecutorResult DoAction() = 0;

public:
	int32 Priority = 0;
	uint64 RegistrationTime = 0;

	FString Name;
	TArray<FString> Pedigree;
};

class FMissionMultiTaskBase : public FMissionTaskBase
{
public:
	FMissionMultiTaskBase();
	~FMissionMultiTaskBase() {}
	virtual void Clear() override;
	virtual void SetData(TSharedPtr<FMissionTaskInfo> inTaskInfo) override;
	virtual ETaskExecutorResult DoAction() override;

	void AddSubTask(TSharedRef<FMissionTaskBase> inSubTask);
	int32 GetStep() { return Step; }
	int32 GetTaskCount() { return SubTaskList.Num(); }
	TSharedPtr<FMissionTaskBase> GetTaskWrapper(int32 inIndex)
	{
		if (false == SubTaskList.IsValidIndex(inIndex))
			return nullptr;

		return SubTaskList[inIndex];
	}

protected:
	TArray<TSharedPtr<FMissionTaskBase>> SubTaskList;
	TSharedPtr<FMissionTaskBase> OccupiedTask;

	TSharedPtr<FMissionTaskInfo> TaskInfo;
	int32 Step = 0;
	EInGameTaskType TaskType = EInGameTaskType::NONE;
	FString Title = TEXT("");
	FString Desc = TEXT("");
};

class FMissionTask_Spawn : public FMissionTaskBase
{
public:
	FMissionTask_Spawn();
	~FMissionTask_Spawn() {}

	virtual void Clear() {}
	virtual void SetData(TSharedPtr<FMissionTaskInfo> inTaskInfo) override;
	virtual ETaskExecutorResult DoAction() override;

public:
	uint64 ActorSN = 0;
	int32 ActorID = 0;
	int32 TableID = 0;
	
	EInGameSpawnType SpawnType;
	int32 TileIndex = -1;
};

class FMissionTask_MarchOn : public FMissionTaskBase
{
public:
	FMissionTask_MarchOn();
	~FMissionTask_MarchOn() {}

	virtual void Clear() {}
	virtual void SetData(TSharedPtr<FMissionTaskInfo> inTaskInfo) override;
	virtual ETaskExecutorResult DoAction() override;

public:
	int32 ActorID = 0;
	int32 TileIndex = -1;
};

class FMissionTask_SetCamera : public FMissionTaskBase
{
public:
	FMissionTask_SetCamera();
	~FMissionTask_SetCamera() {}

	virtual void Clear() {}
	virtual void SetData(TSharedPtr<FMissionTaskInfo> inTaskInfo);
	virtual ETaskExecutorResult DoAction();

public:
	int32 TileIndex = -1;
};


class FMissionTask_PlayDialog : public FMissionTaskBase
{
public:
	FMissionTask_PlayDialog();
	~FMissionTask_PlayDialog() {}

	virtual void Clear() {}
	virtual void SetData(TSharedPtr<FMissionTaskInfo> inTaskInfo);
	virtual ETaskExecutorResult DoAction();

public:
	int32 TileIndex = -1;
	int32 DialogID = 0;
};

