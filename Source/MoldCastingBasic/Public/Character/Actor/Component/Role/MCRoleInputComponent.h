// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Character/Actor/Component/Role/MCRoleInputTypes.h"
#include "EnhancedInputComponent.h"
#include "MoverSimulationTypes.h"
#include "MCRoleInputComponent.generated.h"

class AMCRoleCharacter;
class UEnhancedInputLocalPlayerSubsystem;
class UMCRoleInputConfig;
class UMCRoleMoverComponent;
struct FInputActionValue;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FMCRoleInputActionEvent);

UCLASS(Transient, Config = Input)
class MOLDCASTINGBASIC_API UMCRoleInputComponent
	: public UEnhancedInputComponent
	, public IMoverInputProducerInterface
{
	GENERATED_BODY()

public:
	UMCRoleInputComponent(const FObjectInitializer& ObjectInitializer);

	bool InitializeRoleInput(
		AMCRoleCharacter* InRoleCharacter,
		UMCRoleMoverComponent* InRoleMoverComponent,
		UMCRoleInputConfig* InInputConfig);

	UFUNCTION(BlueprintPure, Category = "Role|Input")
	const FMCRoleInputState& GetInputState() const;

	virtual void ProduceInput_Implementation(int32 SimTimeMs, FMoverInputCmdContext& InputCmdResult) override;

	UPROPERTY(BlueprintAssignable, Category = "Role|Input")
	FMCRoleInputActionEvent OnTraverseRequested;

	UPROPERTY(BlueprintAssignable, Category = "Role|Input")
	FMCRoleInputActionEvent OnInteractRequested;

protected:
	virtual void OnUnregister() override;

private:
	void AddDefaultMappingContext();

	void RemoveDefaultMappingContext();

	void BindInputActions();

	void OnMoveTriggered(const FInputActionValue& Value);

	void OnMoveCompleted(const FInputActionValue& Value);

	void OnMoveWorldSpaceTriggered(const FInputActionValue& Value);

	void OnMoveWorldSpaceCompleted(const FInputActionValue& Value);

	void OnLookTriggered(const FInputActionValue& Value);

	void OnLookCompleted(const FInputActionValue& Value);

	void OnLookGamepadTriggered(const FInputActionValue& Value);

	void OnLookGamepadCompleted(const FInputActionValue& Value);

	void OnJumpStarted(const FInputActionValue& Value);

	void OnJumpCompleted(const FInputActionValue& Value);

	void OnCrouchStarted(const FInputActionValue& Value);

	void OnCrouchCompleted(const FInputActionValue& Value);

	void OnSprintStarted(const FInputActionValue& Value);

	void OnSprintCompleted(const FInputActionValue& Value);

	void OnWalkStarted(const FInputActionValue& Value);

	void OnWalkCompleted(const FInputActionValue& Value);

	void OnStrafeStarted(const FInputActionValue& Value);

	void OnStrafeCompleted(const FInputActionValue& Value);

	void OnAimStarted(const FInputActionValue& Value);

	void OnAimCompleted(const FInputActionValue& Value);

	void OnTraverseStarted(const FInputActionValue& Value);

	void OnInteractStarted(const FInputActionValue& Value);

	void ApplyLookInput(const FVector2D& LookInput, const FVector2D& LookRate);

	UPROPERTY(Transient)
	TObjectPtr<AMCRoleCharacter> RoleCharacter;

	UPROPERTY(Transient)
	TObjectPtr<UMCRoleMoverComponent> RoleMoverComponent;

	UPROPERTY(Transient)
	TObjectPtr<UMCRoleInputConfig> InputConfig;

	UPROPERTY(Transient)
	TWeakObjectPtr<UEnhancedInputLocalPlayerSubsystem> EnhancedInputSubsystem;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Role|Input", meta = (AllowPrivateAccess = "true"))
	FMCRoleInputState InputState;
};
