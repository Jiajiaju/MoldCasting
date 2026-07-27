// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EnhancedInputComponent.h"
#include "MCRoleInputComponent.generated.h"

UCLASS(Transient, Config = Input)
class MOLDCASTINGBASIC_API UMCRoleInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()

public:
	UMCRoleInputComponent(const FObjectInitializer& ObjectInitializer);
};
