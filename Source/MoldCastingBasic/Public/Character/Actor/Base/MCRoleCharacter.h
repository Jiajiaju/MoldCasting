// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MCBaseCharacter.h"
#include "MCRoleCharacter.generated.h"

class UMCRoleInputComponent;
class UMCRoleInputConfig;
class UMCRoleMoverComponent;

UCLASS()
class MOLDCASTINGBASIC_API AMCRoleCharacter : public AMCBaseCharacter
{
	GENERATED_BODY()

public:
	AMCRoleCharacter(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintPure, Category = "Character|Input")
	UMCRoleInputComponent* GetRoleInputComponent() const;

	UFUNCTION(BlueprintPure, Category = "Character|Movement")
	UMCRoleMoverComponent* GetRoleMoverComponent() const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Input")
	TObjectPtr<UMCRoleInputConfig> RoleInputConfig;
};
