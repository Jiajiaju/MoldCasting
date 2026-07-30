// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Service/MCCharacterService.h"
#include "Service/MCDataTableService.h"
#include "Service/MCDebugService.h"
#include "Service/MCLogService.h"
#include "Service/MCUIService.h"
#include "MCGameInstance.generated.h"

UCLASS()
class MOLDCASTINGBASIC_API UMCGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, Transient, Category = "Service")
	TObjectPtr<UMCLogService> LogService = nullptr;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "Service")
	TObjectPtr<UMCDebugService> DebugService = nullptr;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "Service")
	TObjectPtr<UMCDataTableService> DataTableService = nullptr;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "Service")
	TObjectPtr<UMCCharacterService> CharacterService = nullptr;

	UPROPERTY(BlueprintReadOnly, Transient, Category = "Service")
	TObjectPtr<UMCUIService> UIService = nullptr;

protected:
	virtual void OnStart() override;

public:
	UFUNCTION(BlueprintCallable)
	static UMCGameInstance* GetInstance();

	virtual void Init() override;
};
