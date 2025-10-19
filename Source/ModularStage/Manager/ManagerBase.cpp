#include "ManagerBase.h"
#include "Subsystems/SubsystemCollection.h"

void UManagerBase::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	if (!FCoreUObjectDelegates::PreLoadMap.IsBoundToObject(this))
	{
		FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UManagerBase::OnPreLoadMap);
	}

	if (!FCoreUObjectDelegates::PostLoadMapWithWorld.IsBoundToObject(this))
	{
		FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UManagerBase::OnPostLoadMap);
	}
}

void UManagerBase::Deinitialize()
{
	FCoreUObjectDelegates::PreLoadMap.RemoveAll(this);
	FCoreUObjectDelegates::PostLoadMapWithWorld.RemoveAll(this);
	Super::Deinitialize();
}

bool UManagerBase::ShouldCreateSubsystem(UObject* Outer) const
{
	if (!Super::ShouldCreateSubsystem(Outer))
	{
		return false;
	}

	// Do not create an instance of UManagerBase directly.
	// It is intended to be a base class for other managers.
	// Only child classes should be instantiated.
	return GetClass() != UManagerBase::StaticClass();
}

void UManagerBase::OnPreLoadMap(const FString& string)
{
	IsLoading = true;
}

void UManagerBase::OnPostLoadMap(UWorld* world)
{
	IsLoading = false;
}