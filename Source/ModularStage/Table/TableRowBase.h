#pragma once

#include "Engine/DataTable.h"
#include "Engine/EngineTypes.h"
#include "TableEnum.h"
#include "TableRowBase.generated.h"

class UDataTable;

USTRUCT(BlueprintInternalUseOnly)
struct FTableRowStuffInt32 : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	FTableRowStuffInt32() {}
	virtual ~FTableRowStuffInt32() {}
	typedef int32 KeyType;
	static KeyType GetKey(const FName& RowName)
	{
		return FCString::Atoi(*RowName.ToString());
	}
};

// TableRow에 자신의 RowID 정보를 자동으로 세팅해주는 구조체 (TableRow에 ID(Int32)를 포함해야 할 경우만 사용)
USTRUCT(BlueprintInternalUseOnly)
struct FTableRowStuffInt32_OwnID : public FTableRowStuffInt32
{
	GENERATED_BODY()

public:
	UPROPERTY()
		int32 ID = 0;

public:
	// CSV 데이터 Import시 ID 세팅
	virtual void OnPostDataImport(const UDataTable* InDataTable, const FName InRowName, TArray<FString>& OutCollectedImportProblems) override
	{
		ID = FCString::Atoi(*InRowName.ToString());
	}

#if WITH_EDITOR
	// Row 데이터 변경시 ID 세팅 (RowName 변경시도 호출되게 엔진코드 수정)
	virtual void OnDataTableChanged(const UDataTable* InDataTable, const FName InRowName) override
	{
		ID = FCString::Atoi(*InRowName.ToString());
	}
#endif
};

USTRUCT(BlueprintInternalUseOnly)
struct FTableRowStuffInt64 : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	FTableRowStuffInt64() {}
	virtual ~FTableRowStuffInt64() {}
	typedef int64 KeyType;
	static KeyType GetKey(const FName& RowName)
	{
		return FCString::Atoi64(*RowName.ToString());
	}
};

// TableRow에 자신의 RowID 정보를 자동으로 세팅해주는 구조체 (TableRow에 ID(Int64)를 포함해야 할 경우만 사용)
USTRUCT(BlueprintInternalUseOnly)
struct FTableRowStuffInt64_OwnID : public FTableRowStuffInt64
{
	GENERATED_BODY()

public:
	UPROPERTY()
		int64 ID = 0;

public:
	// CSV 데이터 Import시 ID 세팅
	virtual void OnPostDataImport(const UDataTable* InDataTable, const FName InRowName, TArray<FString>& OutCollectedImportProblems) override
	{
		ID = FCString::Atoi64(*InRowName.ToString());
	}

#if WITH_EDITOR
	// Row 데이터 변경시 ID 세팅 (RowName 변경시도 호출되게 엔진코드 수정)
	virtual void OnDataTableChanged(const UDataTable* InDataTable, const FName InRowName) override
	{
		ID = FCString::Atoi64(*InRowName.ToString());
	}
#endif
};

USTRUCT(BlueprintInternalUseOnly)
struct FTableRowStuffString : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
public:
	FTableRowStuffString() {}
	virtual ~FTableRowStuffString() {}
	typedef FString KeyType;
	static KeyType GetKey(const FName& RowName)
	{
		return RowName.ToString();
	}
};

