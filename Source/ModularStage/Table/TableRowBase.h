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

USTRUCT(BlueprintInternalUseOnly)
struct FTableRowStuffInt32_OwnID : public FTableRowStuffInt32
{
	GENERATED_BODY()

public:
	UPROPERTY()
		int32 ID = 0;

public:
	virtual void OnPostDataImport(const UDataTable* InDataTable, const FName InRowName, TArray<FString>& OutCollectedImportProblems) override
	{
		ID = FCString::Atoi(*InRowName.ToString());
	}

#if WITH_EDITOR
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

USTRUCT(BlueprintInternalUseOnly)
struct FTableRowStuffInt64_OwnID : public FTableRowStuffInt64
{
	GENERATED_BODY()

public:
	UPROPERTY()
		int64 ID = 0;

public:
	virtual void OnPostDataImport(const UDataTable* InDataTable, const FName InRowName, TArray<FString>& OutCollectedImportProblems) override
	{
		ID = FCString::Atoi64(*InRowName.ToString());
	}

#if WITH_EDITOR
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

