#include "GameInstance_ModularStage.h"

UGameInstance_ModularStage* UGameInstance_ModularStage::_Instance = nullptr;

UGameInstance_ModularStage::UGameInstance_ModularStage()
{
	_Instance = this;
}

void UGameInstance_ModularStage::Init()
{
	Super::Init();

	FGlobalEventSystem::Init();
}

void UGameInstance_ModularStage::Shutdown()
{
	FGlobalEventSystem::Shutdown();

	Super::Shutdown();
}
