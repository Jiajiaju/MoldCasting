// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MCRoleInputConfig.generated.h"

class UInputAction;
class UInputMappingContext;

UCLASS(BlueprintType)
class MOLDCASTINGBASIC_API UMCRoleInputConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Role|Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Role|Input")
	int32 MappingPriority = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Role|Input|Actions")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Role|Input|Actions")
	TObjectPtr<UInputAction> MoveWorldSpaceAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Role|Input|Actions")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Role|Input|Actions")
	TObjectPtr<UInputAction> LookGamepadAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Role|Input|Actions")
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Role|Input|Actions")
	TObjectPtr<UInputAction> CrouchAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Role|Input|Actions")
	TObjectPtr<UInputAction> SprintAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Role|Input|Actions")
	TObjectPtr<UInputAction> WalkAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Role|Input|Actions")
	TObjectPtr<UInputAction> StrafeAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Role|Input|Actions")
	TObjectPtr<UInputAction> AimAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Role|Input|Actions")
	TObjectPtr<UInputAction> TraverseAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Role|Input|Actions")
	TObjectPtr<UInputAction> InteractAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Role|Input|Look")
	FVector2D GamepadLookRate = FVector2D(300.0, 165.0);
};
