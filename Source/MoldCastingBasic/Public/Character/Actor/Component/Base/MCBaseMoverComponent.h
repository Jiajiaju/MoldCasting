// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DefaultMovementSet/CharacterMoverComponent.h"
#include "MCBaseMoverComponent.generated.h"

UCLASS(BlueprintType, Blueprintable, ClassGroup = Movement, meta = (BlueprintSpawnableComponent))
class MOLDCASTINGBASIC_API UMCBaseMoverComponent : public UCharacterMoverComponent
{
	GENERATED_BODY()

public:
	UMCBaseMoverComponent();
};
