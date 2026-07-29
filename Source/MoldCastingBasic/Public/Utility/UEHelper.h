#pragma once

#include "CoreMinimal.h"

class MOLDCASTINGBASIC_API UEHelper
{
private:
	UEHelper() {}

public:
	template<typename IInterfaceType, typename UInterfaceType>
	static IInterfaceType* InterfaceCast(UObject* UEObject)
	{
		// static_assert(TIsDerivedFrom<UInterfaceType, UInterface>::Value);

		if (!IsValid(UEObject)) return nullptr;
		if (!UEObject->GetClass()->ImplementsInterface(UInterfaceType::StaticClass())) return nullptr;
		return Cast<IInterfaceType>(UEObject);
	}

	template<typename EnumType>
	static FString EnumToString(EnumType EnumValue)
	{
		return StaticEnum<EnumType>()->GetNameStringByValue(static_cast<uint8>(EnumValue));
	}

	template<typename ActorClass>
	static EWorldType::Type GetWorldType(ActorClass* InActor)
	{
		static_assert(TIsDerivedFrom<ActorClass, AActor>::Value, "UEHelper::GetWorldType Template Must a AActor's Sub Class!");
		if (!IsValid(InActor)) return EWorldType::Type::None;

		const UWorld* World = InActor->GetWorld();
		if (!IsValid(World)) return EWorldType::Type::None;

		return World->WorldType;
	}
};
