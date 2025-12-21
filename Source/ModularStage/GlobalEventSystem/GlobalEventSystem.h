// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/GlobalEventListener.h"

class MODULARSTAGE_API FGlobalEventSystem
{
public:
	static void Init()
	{
		UE_LOG(LogTemp, Log, TEXT("[GlobalEventSystem] Ready"));
	}

	static void AddEventListenerAll(UObject* InListener)
	{
		ListenerHasValidEventAction(InListener, [InListener](const UClass* InterfaceClass)
			{
				PrivateHandler->AddEventListenerInternal(InterfaceClass->GetFName(), InListener);
			});
	}

	static void RemoveEventListenerAll(UObject* InListener)
	{
		ListenerHasValidEventAction(InListener, [InListener](const UClass* InterfaceClass)
			{
				PrivateHandler->RemoveEventListenerInternal(InterfaceClass->GetFName(), InListener);
			});
	}

	template <class T, class = typename TEnableIf<TIsIInterface<T>::Value, IGlobalEventListener>::Type>
	static void AddEventListener(UObject* InListener)
	{
		PrivateHandler->AddEventListenerInternal(GetKey<T>(), InListener);
	}

	template <class T, class = typename TEnableIf<TIsIInterface<T>::Value, IGlobalEventListener>::Type>
	static void RemoveEventListener(UObject* InListener)
	{
		PrivateHandler->RemoveEventListenerInternal(GetKey<T>(), InListener);
	}

	template <class T, class = typename TEnableIf<TIsIInterface<T>::Value, IGlobalEventListener>::Type>
	static void ExecuteEvent(TFunction<void(T*)> InFunctor)
	{
		PrivateHandler->ExecuteEventInternal(InFunctor);
	}

	static void Shutdown()
	{
		UE_LOG(LogTemp, Log, TEXT("[GlobalEventSystem] Shutdown"));
		PrivateHandler->EventListenerMap.Empty();
	}

private:
	static void ListenerHasValidEventAction(UObject* InListener, TFunction<void(const UClass*)> InFunctor)
	{
		const UClass* CurrentClass = InListener->GetClass();

		//------------------------------------------------------------------------
		for (; CurrentClass; CurrentClass = CurrentClass->GetSuperClass())
		{

#ifdef __DEBUG_GES__
			//UE_LOG(LogTemp, Log, TEXT("[GlobalEventSystem] class name ==> %s"), *CurrentClass->GetFName().ToString());
#endif

			for (TArray<FImplementedInterface>::TConstIterator It(CurrentClass->Interfaces); It; ++It)
			{
				const UClass* InterfaceClass = It->Class;

#ifdef __DEBUG_GES__				
				//UE_LOG(LogTemp, Log, TEXT("[GlobalEventSystem] interface name ==> %s"), *InterfaceClass->GetFName().ToString());
#endif

				if (InterfaceClass && InterfaceClass->IsChildOf<UGlobalEventListener>())
				{
					InFunctor(InterfaceClass);
				}
			}
		}
	}

	template <class T>
	static FName GetKey()
	{
		UClass* interfaceClass = T::UClassType::StaticClass();
		check(interfaceClass);
		return interfaceClass->GetFName();
		/*
		size_t hash = typeid(T).hash_code();

#ifdef __DEBUG_GES__
		FString Name(typeid(T).name());
		UE_LOG(LogTemp, Log, TEXT("%s to hash => %zu"), *Name, hash);
#endif
		return hash;
		*/
	}

private:
	void AddEventListenerInternal(FName key, UObject* InListener)
	{
		if (EventListenerMap.Contains(key) == false)
		{
			EventListenerMap.Add(key);
		}

		if (EventListenerMap[key].Contains(InListener))
		{
			//UE_LOG(LogTemp, Warning, TEXT("Already Registered GlobalEventSystem Event! Key => %s, Listener => %s"), *key.ToString(), *InListener->GetFName().ToString());
			return;
		}

		EventListenerMap[key].Add(InListener);
	}

	void RemoveEventListenerInternal(FName key, UObject* InListener)
	{
		if (EventListenerMap.Contains(key))
		{
			EventListenerMap[key].Remove(InListener);

			if (EventListenerMap[key].Num() == 0)
			{
				EventListenerMap.Remove(key);
			}
		}
	}

	template <class T>
	void ExecuteEventInternal(TFunction<void(T*)> InFunctor)
	{
		auto key = GetKey<T>();

		if (EventListenerMap.Contains(key))
		{
			TSet<TWeakObjectPtr<UObject>> ToRemove;
			TSet<TWeakObjectPtr<UObject>>& Listeners = EventListenerMap[key];

			for (const auto& WeakListener : Listeners)
			{
				if (WeakListener.IsValid())
				{
					UObject* Listener = WeakListener.Get();
					if (IsCondition<T>(Listener))
					{
						InFunctor(Cast<T>(Listener));
					}
				}
				else
				{
					ToRemove.Add(WeakListener);
				}
			}

			for (const auto& WeakListener : ToRemove)
			{
				Listeners.Remove(WeakListener);
			}
		}
	}

	template <class T, class = typename TEnableIf<TIsIInterface<T>::Value, IGlobalEventListener>::Type>
	bool IsCondition(UObject* InListener)
	{
		if (T* listener = Cast<T>(InListener))
		{
			return listener->IsCondition();
		}

		return InListener != nullptr;
	}

private:
	static TSharedPtr<FGlobalEventSystem> PrivateHandler;

	TMap<FName, TSet<TWeakObjectPtr<UObject>>> EventListenerMap;
};

/*

//# GlobalEventInterfaces.h
//------------------------------------------------------------------------
UINTERFACE()
class UEventExample : public UGlobalEventListener
{
	GENERATED_BODY()
};

class IEventExample : public IGlobalEventListener
{
	GENERATED_BODY()

	public:
	virtual void OnEventExample(int32 InVal) = 0;
};
//------------------------------------------------------------------------

//------------------------------------------------------------------------
void ...()
{
	// implements
	// FGlobalEventSystem::AddEventListenerAll(this);

	OR

	// FGlobalEventSystem::AddEventListener<IEventExample>(this);
}
//------------------------------------------------------------------------

//------------------------------------------------------------------------
void ...()
{

	// implements
	// FGlobalEventSystem::RemoveEventListenerAll(this);

	OR

	
	// FGlobalEventSystem::RemoveEventListener<IEventExample>(this);
}
//------------------------------------------------------------------------


//------------------------------------------------------------------------
...
int32 exampleVal = 1;
FGlobalEventSystem::ExecuteEvent<IEventExample>([exampleVal](IEventExample* listener)
{
	listener->OnExample(exampleVal);
});
...
//------------------------------------------------------------------------

*/