#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ModularStage/GameInstance_ModularStage.h"
#include "ManagerBase.generated.h"

UCLASS(abstract)
class MODULARSTAGE_API UManagerBase : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	// TEnableIf를 사용하여 UManagerBase 파생 클래스에만 이 함수가 존재하도록 제한합니다.
	template <typename T, typename = typename TEnableIf<TIsDerivedFrom<T, UManagerBase>::IsDerived>::Type>
	static T* GetManager()
	{
		if (UGameInstance_ModularStage* GI = UGameInstance_ModularStage::Get())
		{
			return GI->GetSubsystem<T>();
		}

		return nullptr;
	}

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

protected:
	void OnPreLoadMap(const FString& string);
	void OnPostLoadMap(UWorld* world);

protected:
	bool IsLoading = false;
};



