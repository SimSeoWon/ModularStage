#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ModularStage/Table/TableEnum.h"
#include "MissionTaskBase.generated.h"

struct FMissionTaskInfo;

UCLASS(Abstract)
class MODULARSTAGE_API UMissionTaskBase : public UObject
{
	GENERATED_BODY()

public:
	UMissionTaskBase();

	virtual void Clear();

	virtual void SetData(const FMissionTaskInfo& InTaskInfo);

	virtual ETaskExecutorResult DoAction() PURE_VIRTUAL(UMissionTaskBase::DoAction, return ETaskExecutorResult::Fail;);

	virtual void Complete();

	virtual void RollBack();

public:
	UPROPERTY()
	int32 Priority = 0;

	UPROPERTY()
	uint64 RegistrationTime = 0;
};

