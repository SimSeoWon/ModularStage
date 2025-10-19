#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GlobalEventSystem/GlobalEventSystem.h"
#include "GameInstance_ModularStage.generated.h"

/**
 * 
 */
UCLASS()
class MODULARSTAGE_API UGameInstance_ModularStage : public UGameInstance
{
	GENERATED_BODY()

public:
	UGameInstance_ModularStage();

	virtual void Init() override;
	virtual void Shutdown() override;

	static UGameInstance_ModularStage* Get() { return _Instance; }

private:
	static UGameInstance_ModularStage* _Instance;
	
};