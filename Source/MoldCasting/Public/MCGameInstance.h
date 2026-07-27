// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Service/MCCharacterService.h"
#include "MCGameInstance.generated.h"

UCLASS()
class MOLDCASTING_API UMCGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	TObjectPtr<UMCCharacterService> CharacterService = nullptr;

protected:
	virtual void OnStart() override;

public:
	UFUNCTION(BlueprintCallable)
	static UMCGameInstance* GetInstance();

	virtual void Init() override;
};
