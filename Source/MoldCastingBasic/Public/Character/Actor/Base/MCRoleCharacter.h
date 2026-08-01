// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MCBaseCharacter.h"
#include "MCRoleCharacter.generated.h"

class UMCDataAsset_Input;
class UMCRoleInputComponent;
class UMCRoleMoverComponent;

UCLASS()
class MOLDCASTINGBASIC_API AMCRoleCharacter : public AMCBaseCharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character|Input")
	TObjectPtr<UMCDataAsset_Input> RoleInputConfig = nullptr;

public:
	AMCRoleCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual void UnPossessed() override;

	UFUNCTION(BlueprintPure, Category = "Character|Input")
	UMCRoleInputComponent* GetRoleInputComponent() const;

	UFUNCTION(BlueprintPure, Category = "Character|Movement")
	UMCRoleMoverComponent* GetRoleMoverComponent() const;
};
