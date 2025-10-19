#pragma once

#include "CoreMinimal.h"
#include "ManagerBase.h"
#include "Manager_TaskExecutor.generated.h"

class FMissionTaskExecutor;

UCLASS()
class MODULARSTAGE_API UManager_TaskExecutor : public UManagerBase
{
	GENERATED_BODY()

public:

	static UManager_TaskExecutor* Get();

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

protected:
	TArray<TSharedPtr<FMissionTaskExecutor>> MissionExecutors;
};
