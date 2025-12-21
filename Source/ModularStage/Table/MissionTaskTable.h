#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UObject/UObjectGlobals.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Package.h"
#include "TableRowBase.h"

#include "MissionTaskTable.generated.h"


USTRUCT(BlueprintType)
struct MODULARSTAGE_API FMissionTaskInfo
{
	GENERATED_USTRUCT_BODY()
public:
	FMissionTaskInfo();
	FMissionTaskInfo(const FMissionTaskInfo& obj);

public:
	UPROPERTY(EditAnywhere)
	int32 Step = 0;

	UPROPERTY(EditAnywhere)
	FString Title;

	UPROPERTY(EditAnywhere)
	FString Desc;

	UPROPERTY(EditAnywhere)
	FGuid ParentID;

	UPROPERTY(EditAnywhere)
	FGuid SerializeID;

	UPROPERTY(EditAnywhere)
	TArray <int32> IntegerParamList;

	UPROPERTY(EditAnywhere)
	TArray <FString> StringParamList;
};


USTRUCT(BlueprintType)
struct MODULARSTAGE_API FMissionTaskTable : public FTableRowStuffInt32_OwnID
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MissionTask")
	FName TaskId;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MissionTask")
	FName TargetActorTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MissionTask")
	int32 Order;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "MissionTask")
	TArray<FMissionTaskInfo> TaskList;

};
