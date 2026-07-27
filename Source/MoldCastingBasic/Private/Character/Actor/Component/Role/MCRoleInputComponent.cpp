// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Actor/Component/Role/MCRoleInputComponent.h"

#include "Character/Actor/Base/MCRoleCharacter.h"
#include "Character/Actor/Component/Role/MCRoleInputConfig.h"
#include "Character/Actor/Component/Role/MCRoleMoverComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/PlayerController.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "MoverDataModelTypes.h"

namespace MCRoleInput
{
	constexpr double MaxDirectionalInputMagnitude = 1.0;
}

UMCRoleInputComponent::UMCRoleInputComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

bool UMCRoleInputComponent::InitializeRoleInput(
	AMCRoleCharacter* InRoleCharacter,
	UMCRoleMoverComponent* InRoleMoverComponent,
	UMCRoleInputConfig* InInputConfig)
{
	if (!ensure(IsValid(InRoleCharacter))
		|| !ensure(IsValid(InRoleMoverComponent))
		|| !ensure(IsValid(InInputConfig)))
	{
		return false;
	}

	RoleCharacter = InRoleCharacter;
	RoleMoverComponent = InRoleMoverComponent;
	InputConfig = InInputConfig;

	ClearActionBindings();
	BindInputActions();
	AddDefaultMappingContext();

	RoleMoverComponent->SetRoleInputProducer(this);
	return true;
}

const FMCRoleInputState& UMCRoleInputComponent::GetInputState() const
{
	return InputState;
}

void UMCRoleInputComponent::ProduceInput_Implementation(
	int32 SimTimeMs,
	FMoverInputCmdContext& InputCmdResult)
{
	FCharacterDefaultInputs& CharacterInputs =
		InputCmdResult.InputCollection.FindOrAddMutableDataByType<FCharacterDefaultInputs>();
	FMCRoleMoverInputs& RoleInputs =
		InputCmdResult.InputCollection.FindOrAddMutableDataByType<FMCRoleMoverInputs>();

	if (!IsValid(RoleCharacter))
	{
		return;
	}

	const FRotator ControlRotation = RoleCharacter->GetControlRotation();
	const FRotator ControlYawRotation(0.0, ControlRotation.Yaw, 0.0);
	const FVector LocalMoveInput(InputState.MoveInput.Y, InputState.MoveInput.X, 0.0);
	const FVector ClampedMoveInput =
		LocalMoveInput.GetClampedToMaxSize(MCRoleInput::MaxDirectionalInputMagnitude);
	const FVector WorldMoveInput = InputState.bMoveInputIsWorldSpace
		? ClampedMoveInput
		: ControlYawRotation.RotateVector(ClampedMoveInput);

	CharacterInputs.ControlRotation = ControlRotation;
	CharacterInputs.SetMoveInput(EMoveInputType::DirectionalIntent, WorldMoveInput);
	CharacterInputs.OrientationIntent = InputState.bStrafePressed || InputState.bAimPressed
		? ControlYawRotation.Vector()
		: WorldMoveInput.GetSafeNormal();
	CharacterInputs.bIsJumpPressed = InputState.bJumpPressed;
	CharacterInputs.bIsJumpJustPressed = InputState.bJumpJustPressed;

	RoleInputs.bWantsToCrouch = InputState.bCrouchPressed;
	RoleInputs.bWantsToSprint = InputState.bSprintPressed;
	RoleInputs.bWantsToWalk = InputState.bWalkPressed;
	RoleInputs.bWantsToStrafe = InputState.bStrafePressed;
	RoleInputs.bWantsToAim = InputState.bAimPressed;
	RoleInputs.bTraverseJustPressed = InputState.bTraverseJustPressed;
	RoleInputs.bInteractJustPressed = InputState.bInteractJustPressed;

	InputState.bJumpJustPressed = false;
	InputState.bTraverseJustPressed = false;
	InputState.bInteractJustPressed = false;
}

void UMCRoleInputComponent::OnUnregister()
{
	RemoveDefaultMappingContext();

	if (IsValid(RoleMoverComponent))
	{
		RoleMoverComponent->ClearRoleInputProducer(this);
	}

	RoleCharacter = nullptr;
	RoleMoverComponent = nullptr;
	InputConfig = nullptr;

	Super::OnUnregister();
}

void UMCRoleInputComponent::AddDefaultMappingContext()
{
	if (!IsValid(RoleCharacter)
		|| !IsValid(InputConfig)
		|| !IsValid(InputConfig->DefaultMappingContext.Get()))
	{
		return;
	}

	APlayerController* PlayerController = Cast<APlayerController>(RoleCharacter->GetController());
	ULocalPlayer* LocalPlayer = PlayerController ? PlayerController->GetLocalPlayer() : nullptr;
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = LocalPlayer
		? LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()
		: nullptr;

	if (!ensure(IsValid(InputSubsystem)))
	{
		return;
	}

	EnhancedInputSubsystem = InputSubsystem;
	InputSubsystem->AddMappingContext(InputConfig->DefaultMappingContext, InputConfig->MappingPriority);
}

void UMCRoleInputComponent::RemoveDefaultMappingContext()
{
	if (EnhancedInputSubsystem.IsValid()
		&& IsValid(InputConfig)
		&& IsValid(InputConfig->DefaultMappingContext.Get()))
	{
		EnhancedInputSubsystem->RemoveMappingContext(InputConfig->DefaultMappingContext);
	}

	EnhancedInputSubsystem.Reset();
}

void UMCRoleInputComponent::BindInputActions()
{
	if (!ensure(IsValid(InputConfig)))
	{
		return;
	}

	if (InputConfig->MoveAction)
	{
		BindAction(
			InputConfig->MoveAction,
			ETriggerEvent::Triggered,
			this,
			&UMCRoleInputComponent::OnMoveTriggered);
		BindAction(
			InputConfig->MoveAction,
			ETriggerEvent::Completed,
			this,
			&UMCRoleInputComponent::OnMoveCompleted);
	}

	if (InputConfig->MoveWorldSpaceAction)
	{
		BindAction(
			InputConfig->MoveWorldSpaceAction,
			ETriggerEvent::Triggered,
			this,
			&UMCRoleInputComponent::OnMoveWorldSpaceTriggered);
		BindAction(
			InputConfig->MoveWorldSpaceAction,
			ETriggerEvent::Completed,
			this,
			&UMCRoleInputComponent::OnMoveWorldSpaceCompleted);
	}

	if (InputConfig->LookAction)
	{
		BindAction(
			InputConfig->LookAction,
			ETriggerEvent::Triggered,
			this,
			&UMCRoleInputComponent::OnLookTriggered);
		BindAction(
			InputConfig->LookAction,
			ETriggerEvent::Completed,
			this,
			&UMCRoleInputComponent::OnLookCompleted);
	}

	if (InputConfig->LookGamepadAction)
	{
		BindAction(
			InputConfig->LookGamepadAction,
			ETriggerEvent::Triggered,
			this,
			&UMCRoleInputComponent::OnLookGamepadTriggered);
		BindAction(
			InputConfig->LookGamepadAction,
			ETriggerEvent::Completed,
			this,
			&UMCRoleInputComponent::OnLookGamepadCompleted);
	}

	if (InputConfig->JumpAction)
	{
		BindAction(
			InputConfig->JumpAction,
			ETriggerEvent::Started,
			this,
			&UMCRoleInputComponent::OnJumpStarted);
		BindAction(
			InputConfig->JumpAction,
			ETriggerEvent::Completed,
			this,
			&UMCRoleInputComponent::OnJumpCompleted);
	}

	if (InputConfig->CrouchAction)
	{
		BindAction(
			InputConfig->CrouchAction,
			ETriggerEvent::Started,
			this,
			&UMCRoleInputComponent::OnCrouchStarted);
		BindAction(
			InputConfig->CrouchAction,
			ETriggerEvent::Completed,
			this,
			&UMCRoleInputComponent::OnCrouchCompleted);
	}

	if (InputConfig->SprintAction)
	{
		BindAction(
			InputConfig->SprintAction,
			ETriggerEvent::Started,
			this,
			&UMCRoleInputComponent::OnSprintStarted);
		BindAction(
			InputConfig->SprintAction,
			ETriggerEvent::Completed,
			this,
			&UMCRoleInputComponent::OnSprintCompleted);
	}

	if (InputConfig->WalkAction)
	{
		BindAction(
			InputConfig->WalkAction,
			ETriggerEvent::Started,
			this,
			&UMCRoleInputComponent::OnWalkStarted);
		BindAction(
			InputConfig->WalkAction,
			ETriggerEvent::Completed,
			this,
			&UMCRoleInputComponent::OnWalkCompleted);
	}

	if (InputConfig->StrafeAction)
	{
		BindAction(
			InputConfig->StrafeAction,
			ETriggerEvent::Started,
			this,
			&UMCRoleInputComponent::OnStrafeStarted);
		BindAction(
			InputConfig->StrafeAction,
			ETriggerEvent::Completed,
			this,
			&UMCRoleInputComponent::OnStrafeCompleted);
	}

	if (InputConfig->AimAction)
	{
		BindAction(
			InputConfig->AimAction,
			ETriggerEvent::Started,
			this,
			&UMCRoleInputComponent::OnAimStarted);
		BindAction(
			InputConfig->AimAction,
			ETriggerEvent::Completed,
			this,
			&UMCRoleInputComponent::OnAimCompleted);
	}

	if (InputConfig->TraverseAction)
	{
		BindAction(
			InputConfig->TraverseAction,
			ETriggerEvent::Started,
			this,
			&UMCRoleInputComponent::OnTraverseStarted);
	}

	if (InputConfig->InteractAction)
	{
		BindAction(
			InputConfig->InteractAction,
			ETriggerEvent::Started,
			this,
			&UMCRoleInputComponent::OnInteractStarted);
	}
}

void UMCRoleInputComponent::OnMoveTriggered(const FInputActionValue& Value)
{
	InputState.MoveInput = Value.Get<FVector2D>();
	InputState.bMoveInputIsWorldSpace = false;
}

void UMCRoleInputComponent::OnMoveCompleted(const FInputActionValue& Value)
{
	if (!InputState.bMoveInputIsWorldSpace)
	{
		InputState.MoveInput = FVector2D::ZeroVector;
	}
}

void UMCRoleInputComponent::OnMoveWorldSpaceTriggered(const FInputActionValue& Value)
{
	InputState.MoveInput = Value.Get<FVector2D>();
	InputState.bMoveInputIsWorldSpace = true;
}

void UMCRoleInputComponent::OnMoveWorldSpaceCompleted(const FInputActionValue& Value)
{
	if (InputState.bMoveInputIsWorldSpace)
	{
		InputState.MoveInput = FVector2D::ZeroVector;
	}
}

void UMCRoleInputComponent::OnLookTriggered(const FInputActionValue& Value)
{
	InputState.LookInput = Value.Get<FVector2D>();
	ApplyLookInput(InputState.LookInput, FVector2D::UnitVector);
}

void UMCRoleInputComponent::OnLookCompleted(const FInputActionValue& Value)
{
	InputState.LookInput = FVector2D::ZeroVector;
}

void UMCRoleInputComponent::OnLookGamepadTriggered(const FInputActionValue& Value)
{
	InputState.LookInput = Value.Get<FVector2D>();

	if (IsValid(InputConfig))
	{
		const float DeltaSeconds = GetWorld() ? GetWorld()->GetDeltaSeconds() : 0.0f;
		ApplyLookInput(InputState.LookInput, InputConfig->GamepadLookRate * DeltaSeconds);
	}
}

void UMCRoleInputComponent::OnLookGamepadCompleted(const FInputActionValue& Value)
{
	InputState.LookInput = FVector2D::ZeroVector;
}

void UMCRoleInputComponent::OnJumpStarted(const FInputActionValue& Value)
{
	InputState.bJumpJustPressed = !InputState.bJumpPressed;
	InputState.bJumpPressed = true;
}

void UMCRoleInputComponent::OnJumpCompleted(const FInputActionValue& Value)
{
	InputState.bJumpPressed = false;
	InputState.bJumpJustPressed = false;
}

void UMCRoleInputComponent::OnCrouchStarted(const FInputActionValue& Value)
{
	InputState.bCrouchPressed = true;

	if (IsValid(RoleMoverComponent))
	{
		RoleMoverComponent->Crouch();
	}
}

void UMCRoleInputComponent::OnCrouchCompleted(const FInputActionValue& Value)
{
	InputState.bCrouchPressed = false;

	if (IsValid(RoleMoverComponent))
	{
		RoleMoverComponent->UnCrouch();
	}
}

void UMCRoleInputComponent::OnSprintStarted(const FInputActionValue& Value)
{
	InputState.bSprintPressed = true;
}

void UMCRoleInputComponent::OnSprintCompleted(const FInputActionValue& Value)
{
	InputState.bSprintPressed = false;
}

void UMCRoleInputComponent::OnWalkStarted(const FInputActionValue& Value)
{
	InputState.bWalkPressed = true;
}

void UMCRoleInputComponent::OnWalkCompleted(const FInputActionValue& Value)
{
	InputState.bWalkPressed = false;
}

void UMCRoleInputComponent::OnStrafeStarted(const FInputActionValue& Value)
{
	InputState.bStrafePressed = true;
}

void UMCRoleInputComponent::OnStrafeCompleted(const FInputActionValue& Value)
{
	InputState.bStrafePressed = false;
}

void UMCRoleInputComponent::OnAimStarted(const FInputActionValue& Value)
{
	InputState.bAimPressed = true;
}

void UMCRoleInputComponent::OnAimCompleted(const FInputActionValue& Value)
{
	InputState.bAimPressed = false;
}

void UMCRoleInputComponent::OnTraverseStarted(const FInputActionValue& Value)
{
	InputState.bTraverseJustPressed = true;
	OnTraverseRequested.Broadcast();
}

void UMCRoleInputComponent::OnInteractStarted(const FInputActionValue& Value)
{
	InputState.bInteractJustPressed = true;
	OnInteractRequested.Broadcast();
}

void UMCRoleInputComponent::ApplyLookInput(const FVector2D& LookInput, const FVector2D& LookRate)
{
	if (!IsValid(RoleCharacter))
	{
		return;
	}

	RoleCharacter->AddControllerYawInput(LookInput.X * LookRate.X);
	RoleCharacter->AddControllerPitchInput(LookInput.Y * LookRate.Y);
}
