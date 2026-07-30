// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MCGameInstance.generated.h"

class UMCCharacterService;
class UMCUIService;

UCLASS()
class MOLDCASTINGBASIC_API UMCGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
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
