#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UEObjectPool.generated.h"

UINTERFACE(MinimalAPI, NotBlueprintable)
class UObjectPoolElement : public UInterface
{
	GENERATED_BODY()
};

class MOLDCASTINGBASIC_API IObjectPoolElement
{
	GENERATED_BODY()
public:
	virtual void OnBeforeTake_ObjectPool() = 0;
	virtual void OnAfterTake_ObjectPool() = 0;
	virtual void OnBeforeRecycle_ObjectPool() = 0;
	virtual void OnAfterRecycle_ObjectPool() = 0;
	virtual void OnReset_ObjectPool() = 0;
	virtual void OnDefaultInit_ObjectPool() = 0;
};

template<typename T>
class UObjectPool
{
public:

	void PreWarm(int32 InCount, UObject* InOuter = nullptr)
	{
		static_assert(TIsDerivedFrom<T, UObject>::IsDerived, "UXYObjectPool: T must derive from UObject");
		static_assert(TIsDerivedFrom<T, IObjectPoolElement>::IsDerived, "UXYObjectPool: T must implement IXYObjectPoolElement");

		UObject* Outer = IsValid(InOuter) ? InOuter : GetTransientPackage();
		for (int32 Index = 0; Index < InCount; ++Index)
		{
			T* Element = NewObject<T>(Outer);
			IObjectPoolElement* PoolElement = CastChecked<IObjectPoolElement>(Element);
			PoolElement->OnDefaultInit_ObjectPool();
			PoolElement->OnReset_ObjectPool();
			PoolElement->OnAfterRecycle_ObjectPool();
			AvailableElements.Add(Element);
		}
	}

	T* Take(UObject* InOuter = nullptr)
	{
		static_assert(TIsDerivedFrom<T, UObject>::IsDerived, "UXYObjectPool: T must derive from UObject");
		static_assert(TIsDerivedFrom<T, IObjectPoolElement>::IsDerived, "UXYObjectPool: T must implement IXYObjectPoolElement");

		T* Element = nullptr;

		if (AvailableElements.Num() > 0)
		{
			Element = AvailableElements.Pop();
		}
		else
		{
			UObject* Outer = IsValid(InOuter) ? InOuter : GetTransientPackage();
			Element = NewObject<T>(Outer);
			IObjectPoolElement* PoolElement = CastChecked<IObjectPoolElement>(Element);
			PoolElement->OnDefaultInit_ObjectPool();
		}

		IObjectPoolElement* PoolElement = CastChecked<IObjectPoolElement>(Element);
		PoolElement->OnBeforeTake_ObjectPool();
		TakenElements.Add(Element);
		PoolElement->OnAfterTake_ObjectPool();

		return Element;
	}

	void Recycle(T* InElement)
	{
		if (!IsValid(InElement)) return;

		IObjectPoolElement* PoolElement = CastChecked<IObjectPoolElement>(InElement);
		PoolElement->OnBeforeRecycle_ObjectPool();
		TakenElements.Remove(InElement);
		PoolElement->OnReset_ObjectPool();
		AvailableElements.Add(InElement);
		PoolElement->OnAfterRecycle_ObjectPool();
	}

	int32 GetAvailableCount() const { return AvailableElements.Num(); }
	int32 GetTakenCount() const { return TakenElements.Num(); }
	int32 GetTotalCount() const { return AvailableElements.Num() + TakenElements.Num(); }

private:

	TArray<T*> AvailableElements;
	TArray<T*> TakenElements;
};
