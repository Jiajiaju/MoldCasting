// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Actor/Component/Role/MCRoleMoverComponent.h"

#include "MoverSimulationTypes.h"

UMCRoleMoverComponent::UMCRoleMoverComponent()
{
}

void UMCRoleMoverComponent::SetRoleInputProducer(UObject* NewInputProducer)
{
	if (!ensure(IsValid(NewInputProducer))
		|| !ensure(NewInputProducer->GetClass()->ImplementsInterface(
			UMoverInputProducerInterface::StaticClass())))
	{
		return;
	}

	if (IsValid(InputProducer))
	{
		InputProducers.Remove(InputProducer);
	}

	InputProducer = NewInputProducer;
	InputProducers.RemoveAll(
		[](const TObjectPtr<UObject>& Producer)
		{
			return !IsValid(Producer);
		});
	InputProducers.AddUnique(NewInputProducer);
}

void UMCRoleMoverComponent::ClearRoleInputProducer(const UObject* InputProducerToClear)
{
	if (InputProducer != InputProducerToClear)
	{
		return;
	}

	InputProducers.Remove(InputProducer);
	InputProducer = nullptr;
}
