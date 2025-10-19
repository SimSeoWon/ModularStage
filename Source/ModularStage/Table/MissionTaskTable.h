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
	int32 Step = 0; //진행도

	UPROPERTY(EditAnywhere)
	FString Title; // 타이틀

	UPROPERTY(EditAnywhere)
	FString Desc; // 설명

	UPROPERTY(EditAnywhere)
	FGuid ParentID; // 부모 노드의 아이디

	UPROPERTY(EditAnywhere)
	FGuid SerializeID; // 구분자

	UPROPERTY(EditAnywhere)
	TArray <int32> IntegerParamList; // int32 파라미터 목록.

	UPROPERTY(EditAnywhere)
	TArray <FString> StringParamList; // 스트링 파라미터 목록.
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
