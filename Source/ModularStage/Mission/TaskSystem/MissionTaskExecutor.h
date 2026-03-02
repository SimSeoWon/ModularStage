#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Tickable.h"
#include "Engine/DataTable.h"
#include "ModularStage/Table/TableEnum.h"
#include "MissionTaskExecutor.generated.h"

class UMissionTaskBase;
struct FMissionTaskInfo;

enum class EInGameTaskType : uint8;

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnMissionTaskStepChanged, UMissionTaskExecutor*, int32 /*Step*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnMissionTaskCompleted, UMissionTaskExecutor*);

UCLASS()
class MODULARSTAGE_API UMissionTaskExecutor : public UObject, public FTickableGameObject
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	UMissionTaskExecutor(const FObjectInitializer& ObjectInitializer);
	void Initialize(const FDataTableRowHandle& InMissionData);
	bool InitializeFromTaskList(const TArray<FMissionTaskInfo>& InTaskList);
	void Shutdown();

	void StartMission();
	void SetState(EMissionExecutorState InState);
	EMissionExecutorState GetState() const { return CurrentState; }

	bool IsActive() const { return CurrentState == EMissionExecutorState::Running; }
	bool IsCompleted() const { return CurrentState == EMissionExecutorState::Completed; }
	bool IsInitialized() const { return CurrentState != EMissionExecutorState::None; }

	void SetExecutionInfo(EMissionType InType, int32 InOrder) { MissionType = InType; ExecutionOrder = InOrder; }
	EMissionType GetMissionType() const { return MissionType; }
	int32 GetExecutionOrder() const { return ExecutionOrder; }

	// FTickableGameObject Interface
	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;
	virtual bool IsTickable() const override;
	virtual ETickableTickType GetTickableTickType() const override { return ETickableTickType::Conditional; }
	// ~FTickableGameObject Interface

	FOnMissionTaskStepChanged OnStepChanged;
	FOnMissionTaskCompleted OnCompleted;

protected:
	TObjectPtr<UMissionTaskBase> CreateTask(const FMissionTaskInfo& inTaskInfo);
	TObjectPtr<UMissionTaskBase> OnCreateTask_SPAWN_OBJECT(const FMissionTaskInfo& inTaskInfo);
	TObjectPtr<UMissionTaskBase> OnCreateTask_SPAWN_MONSTER(const FMissionTaskInfo& inTaskInfo);
	TObjectPtr<UMissionTaskBase> OnCreateTask_MISSION(const FMissionTaskInfo& inTaskInfo);
protected:
	UPROPERTY()
	TArray<TObjectPtr<UMissionTaskBase>> SequentialTaskList;

	UPROPERTY()
	TObjectPtr<UMissionTaskBase> OccupiedTask = nullptr;

	FDataTableRowHandle MissionData;
	int32 Step = 0;

	EMissionType MissionType = EMissionType::Main;
	int32 ExecutionOrder = 0;
	EMissionExecutorState CurrentState = EMissionExecutorState::None;
};