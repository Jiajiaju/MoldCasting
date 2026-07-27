// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Character/Actor/Component/Base/MCBaseMoverComponent.h"
#include "MCRoleMoverComponent.generated.h"

UCLASS(BlueprintType, Blueprintable, ClassGroup = Movement, meta = (BlueprintSpawnableComponent))
class MOLDCASTINGBASIC_API UMCRoleMoverComponent : public UMCBaseMoverComponent
{
	GENERATED_BODY()

public:
	UMCRoleMoverComponent();

	void SetRoleInputProducer(UObject* NewInputProducer);

	void ClearRoleInputProducer(const UObject* InputProducerToClear);
};
