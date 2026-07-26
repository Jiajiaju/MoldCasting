// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MCBaseService.generated.h"

UCLASS(Abstract)
class MOLDCASTING_API UMCBaseService : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	virtual void Init();
	virtual void OnStart();
};
