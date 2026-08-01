// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Input/MCInputTypes.h"
#include "MCDataAsset_Input.generated.h"

class UInputAction;
class UInputMappingContext;

UCLASS(BlueprintType)
class MOLDCASTINGBASIC_API UMCDataAsset_Input : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Role|Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Role|Input")
	int32 MappingPriority = MCInputMappingPriority::Gameplay;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Role|Input|Actions", meta = (TitleProperty = "InputTag"))
	TArray<FMCInputActionDefinition> InputActions {};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Role|Input|Actions")
	TObjectPtr<UInputAction> MoveAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Role|Input|Actions")
	TObjectPtr<UInputAction> MoveWorldSpaceAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Role|Input|Actions")
	TObjectPtr<UInputAction> LookAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Role|Input|Actions")
	TObjectPtr<UInputAction> LookGamepadAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Role|Input|Actions")
	TObjectPtr<UInputAction> JumpAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Role|Input|Actions")
	TObjectPtr<UInputAction> CrouchAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Role|Input|Actions")
	TObjectPtr<UInputAction> SprintAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Role|Input|Actions")
	TObjectPtr<UInputAction> WalkAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Role|Input|Actions")
	TObjectPtr<UInputAction> StrafeAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Role|Input|Actions")
	TObjectPtr<UInputAction> AimAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Role|Input|Actions")
	TObjectPtr<UInputAction> TraverseAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Role|Input|Actions")
	TObjectPtr<UInputAction> InteractAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Role|Input|Look")
	FVector2D GamepadLookRate = FVector2D(300.0, 165.0);

	const UInputAction* FindInputActionForTag(
		const FGameplayTag& InputTag,
		const UInputAction* LegacyInputAction = nullptr) const;
};
