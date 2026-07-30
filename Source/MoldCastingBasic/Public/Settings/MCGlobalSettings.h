// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "MCGlobalSettings.generated.h"

/**
 *
 */
UCLASS(Config=MCGlobal, DefaultConfig, meta=(DisplayName="Global"))
class MOLDCASTINGBASIC_API UMCGlobalSettings : public UDeveloperSettings
{
	GENERATED_BODY()
public:

	virtual FName GetCategoryName() const override { return TEXT("MC"); }
	virtual FName GetSectionName() const override  { return TEXT("GlobalSettings"); }
};
