#pragma once

#include "CoreMinimal.h"
#include "ManagerBase.h"
#include "Manager_Mission.generated.h"

class UMissionTaskExecutor;

UCLASS()
class MODULARSTAGE_API UManager_Mission : public UManagerBase
{
	GENERATED_BODY()

public:

	static UManager_Mission* Get();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

	void RegisterExecutor(TObjectPtr<UMissionTaskExecutor> ExecutorToRegister);
	void NotifyTaskExecutorCompletion(TObjectPtr<UMissionTaskExecutor> CompletedExecutor); // 이 라인을 추가합니다.

protected:
	virtual void OnPreLoadMap(const FString& string) override;

protected:
		void HandleOnStepChanged(UMissionTaskExecutor* Executor, int32 Step);
		void HandleOnCompleted(UMissionTaskExecutor* Executor);
		void TryStartNextMainMission();
	
	protected:
		UPROPERTY()
		TArray<TObjectPtr<UMissionTaskExecutor>> MainMissionExecutors;

		UPROPERTY()
		TArray<TObjectPtr<UMissionTaskExecutor>> SubMissionExecutors;
	
		int32 CurrentMainMissionOrder = 0;
	};
