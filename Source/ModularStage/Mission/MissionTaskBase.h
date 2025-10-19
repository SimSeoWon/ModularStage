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




/**미션 진행을 관리하는 태스크 원형*/
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
	int32 Step = 0; //진행도
	EInGameTaskType TaskType = EInGameTaskType::NONE; // 태스크 타입.
	FString Title = TEXT(""); // 타이틀
	FString Desc = TEXT(""); // 설명
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
	uint64 ActorSN = 0; // 시리얼 넘버..
	int32 ActorID = 0; // 액터 아이디
	int32 TableID = 0; // 테이블 아이디.
	// 테이블 종류 -> 몬스터, 배경 기물, 플레이어 등..
	EInGameSpawnType SpawnType;
	int32 TileIndex = -1;		// 작동할 타일의 인덱스
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
	int32 ActorID = 0; // 액터 아이디
	int32 TileIndex = -1;		// 목적지의 타일 인덱스
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
	int32 TileIndex = -1;		// 작동할 타일의 인덱스
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
	int32 TileIndex = -1;		// 작동할 타일의 인덱스
	int32 DialogID = 0;
};

