// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Actor/Component/Role/MCRoleInputComponent.h"

#include "Character/Actor/Base/MCRoleCharacter.h"
#include "Character/Actor/Component/Role/MCRoleMoverComponent.h"
#include "Common/MCGameplayTags.h"
#include "DataAsset/Character/MCDataAsset_Input.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Input/MCGameplayInputRouter.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "MoverDataModelTypes.h"
#include "Struct/Input/MCRoleMoverInputs.h"

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
	UMCDataAsset_Input* InInputConfig)
{
	if (!ensure(IsValid(InRoleCharacter))
		|| !ensure(IsValid(InRoleMoverComponent))
		|| !ensure(IsValid(InInputConfig)))
	{
		return false;
	}

	APlayerController* PlayerController = Cast<APlayerController>(InRoleCharacter->GetController());
	if (!ensure(InRoleCharacter->IsLocallyControlled())
		|| !ensure(IsValid(PlayerController)))
	{
		return false;
	}

	ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
	if (!ensure(IsValid(LocalPlayer)))
	{
		return false;
	}

	ShutdownRoleInput();

	RoleCharacter = InRoleCharacter;
	RoleMoverComponent = InRoleMoverComponent;
	InputConfig = InInputConfig;
	GameplayInputRouter = LocalPlayer->GetSubsystem<UMCGameplayInputRouter>();

	if (!ensure(GameplayInputRouter.IsValid()))
	{
		ShutdownRoleInput();
		return false;
	}

	GameplayInputRouter->OnBlockedInputTagsChanged().AddUObject(
		this,
		&UMCRoleInputComponent::HandleBlockedInputTagsChanged);

	BindInputActions();
	AddDefaultMappingContext();

	RoleMoverComponent->SetRoleInputProducer(this);
	return true;
}

void UMCRoleInputComponent::ShutdownRoleInput()
{
	RemoveDefaultMappingContext();

	if (GameplayInputRouter.IsValid())
	{
		GameplayInputRouter->OnBlockedInputTagsChanged().RemoveAll(this);
	}
	GameplayInputRouter.Reset();

	ClearActionBindings();
	ResetRoleInputState();

	if (IsValid(RoleMoverComponent))
	{
		RoleMoverComponent->ClearRoleInputProducer(this);
	}

	RoleCharacter = nullptr;
	RoleMoverComponent = nullptr;
	InputConfig = nullptr;
}

const FMCRoleInputState& UMCRoleInputComponent::GetInputState() const
{
	return InputState;
}

void UMCRoleInputComponent::ResetRoleInputState()
{
	const bool bWasCrouchPressed = InputState.bCrouchPressed;
	InputState = FMCRoleInputState();

	if (bWasCrouchPressed && IsValid(RoleMoverComponent))
	{
		RoleMoverComponent->UnCrouch();
	}
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
	ShutdownRoleInput();

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
	if (!ensure(IsValid(PlayerController)))
	{
		return;
	}

	ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
	if (!ensure(IsValid(LocalPlayer)))
	{
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* InputSubsystem =
		LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
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

	const UInputAction* MoveAction = InputConfig->FindInputActionForTag(
		MCGameplayTags::Input_Gameplay_Move,
		InputConfig->MoveAction);
	if (IsValid(MoveAction))
	{
		BindAction(
			MoveAction,
			ETriggerEvent::Triggered,
			this,
			&UMCRoleInputComponent::OnMoveTriggered);
		BindAction(
			MoveAction,
			ETriggerEvent::Completed,
			this,
			&UMCRoleInputComponent::OnMoveCompleted);
		BindAction(
			MoveAction,
			ETriggerEvent::Canceled,
			this,
			&UMCRoleInputComponent::OnMoveCompleted);
	}

	const UInputAction* MoveWorldSpaceAction = InputConfig->FindInputActionForTag(
		MCGameplayTags::Input_Gameplay_Move_WorldSpace,
		InputConfig->MoveWorldSpaceAction);
	if (IsValid(MoveWorldSpaceAction))
	{
		BindAction(
			MoveWorldSpaceAction,
			ETriggerEvent::Triggered,
			this,
			&UMCRoleInputComponent::OnMoveWorldSpaceTriggered);
		BindAction(
			MoveWorldSpaceAction,
			ETriggerEvent::Completed,
			this,
			&UMCRoleInputComponent::OnMoveWorldSpaceCompleted);
		BindAction(
			MoveWorldSpaceAction,
			ETriggerEvent::Canceled,
			this,
			&UMCRoleInputComponent::OnMoveWorldSpaceCompleted);
	}

	const UInputAction* LookAction = InputConfig->FindInputActionForTag(
		MCGameplayTags::Input_Gameplay_Look_Mouse,
		InputConfig->LookAction);
	if (IsValid(LookAction))
	{
		BindAction(
			LookAction,
			ETriggerEvent::Triggered,
			this,
			&UMCRoleInputComponent::OnLookTriggered);
		BindAction(
			LookAction,
			ETriggerEvent::Completed,
			this,
			&UMCRoleInputComponent::OnLookCompleted);
		BindAction(
			LookAction,
			ETriggerEvent::Canceled,
			this,
			&UMCRoleInputComponent::OnLookCompleted);
	}

	const UInputAction* LookGamepadAction = InputConfig->FindInputActionForTag(
		MCGameplayTags::Input_Gameplay_Look_Gamepad,
		InputConfig->LookGamepadAction);
	if (IsValid(LookGamepadAction))
	{
		BindAction(
			LookGamepadAction,
			ETriggerEvent::Triggered,
			this,
			&UMCRoleInputComponent::OnLookGamepadTriggered);
		BindAction(
			LookGamepadAction,
			ETriggerEvent::Completed,
			this,
			&UMCRoleInputComponent::OnLookGamepadCompleted);
		BindAction(
			LookGamepadAction,
			ETriggerEvent::Canceled,
			this,
			&UMCRoleInputComponent::OnLookGamepadCompleted);
	}

	const UInputAction* JumpAction = InputConfig->FindInputActionForTag(
		MCGameplayTags::Input_Gameplay_Jump,
		InputConfig->JumpAction);
	if (IsValid(JumpAction))
	{
		BindAction(
			JumpAction,
			ETriggerEvent::Started,
			this,
			&UMCRoleInputComponent::OnJumpStarted);
		BindAction(
			JumpAction,
			ETriggerEvent::Completed,
			this,
			&UMCRoleInputComponent::OnJumpCompleted);
		BindAction(
			JumpAction,
			ETriggerEvent::Canceled,
			this,
			&UMCRoleInputComponent::OnJumpCompleted);
	}

	const UInputAction* CrouchAction = InputConfig->FindInputActionForTag(
		MCGameplayTags::Input_Gameplay_Crouch,
		InputConfig->CrouchAction);
	if (IsValid(CrouchAction))
	{
		BindAction(
			CrouchAction,
			ETriggerEvent::Started,
			this,
			&UMCRoleInputComponent::OnCrouchStarted);
		BindAction(
			CrouchAction,
			ETriggerEvent::Completed,
			this,
			&UMCRoleInputComponent::OnCrouchCompleted);
		BindAction(
			CrouchAction,
			ETriggerEvent::Canceled,
			this,
			&UMCRoleInputComponent::OnCrouchCompleted);
	}

	const UInputAction* SprintAction = InputConfig->FindInputActionForTag(
		MCGameplayTags::Input_Gameplay_Sprint,
		InputConfig->SprintAction);
	if (IsValid(SprintAction))
	{
		BindAction(
			SprintAction,
			ETriggerEvent::Started,
			this,
			&UMCRoleInputComponent::OnSprintStarted);
		BindAction(
			SprintAction,
			ETriggerEvent::Completed,
			this,
			&UMCRoleInputComponent::OnSprintCompleted);
		BindAction(
			SprintAction,
			ETriggerEvent::Canceled,
			this,
			&UMCRoleInputComponent::OnSprintCompleted);
	}

	const UInputAction* WalkAction = InputConfig->FindInputActionForTag(
		MCGameplayTags::Input_Gameplay_Walk,
		InputConfig->WalkAction);
	if (IsValid(WalkAction))
	{
		BindAction(
			WalkAction,
			ETriggerEvent::Started,
			this,
			&UMCRoleInputComponent::OnWalkStarted);
		BindAction(
			WalkAction,
			ETriggerEvent::Completed,
			this,
			&UMCRoleInputComponent::OnWalkCompleted);
		BindAction(
			WalkAction,
			ETriggerEvent::Canceled,
			this,
			&UMCRoleInputComponent::OnWalkCompleted);
	}

	const UInputAction* StrafeAction = InputConfig->FindInputActionForTag(
		MCGameplayTags::Input_Gameplay_Strafe,
		InputConfig->StrafeAction);
	if (IsValid(StrafeAction))
	{
		BindAction(
			StrafeAction,
			ETriggerEvent::Started,
			this,
			&UMCRoleInputComponent::OnStrafeStarted);
		BindAction(
			StrafeAction,
			ETriggerEvent::Completed,
			this,
			&UMCRoleInputComponent::OnStrafeCompleted);
		BindAction(
			StrafeAction,
			ETriggerEvent::Canceled,
			this,
			&UMCRoleInputComponent::OnStrafeCompleted);
	}

	const UInputAction* AimAction = InputConfig->FindInputActionForTag(
		MCGameplayTags::Input_Gameplay_Aim,
		InputConfig->AimAction);
	if (IsValid(AimAction))
	{
		BindAction(
			AimAction,
			ETriggerEvent::Started,
			this,
			&UMCRoleInputComponent::OnAimStarted);
		BindAction(
			AimAction,
			ETriggerEvent::Completed,
			this,
			&UMCRoleInputComponent::OnAimCompleted);
		BindAction(
			AimAction,
			ETriggerEvent::Canceled,
			this,
			&UMCRoleInputComponent::OnAimCompleted);
	}

	const UInputAction* TraverseAction = InputConfig->FindInputActionForTag(
		MCGameplayTags::Input_Gameplay_Traverse,
		InputConfig->TraverseAction);
	if (IsValid(TraverseAction))
	{
		BindAction(
			TraverseAction,
			ETriggerEvent::Started,
			this,
			&UMCRoleInputComponent::OnTraverseStarted);
	}

	const UInputAction* InteractAction = InputConfig->FindInputActionForTag(
		MCGameplayTags::Input_Gameplay_Interact,
		InputConfig->InteractAction);
	if (IsValid(InteractAction))
	{
		BindAction(
			InteractAction,
			ETriggerEvent::Started,
			this,
			&UMCRoleInputComponent::OnInteractStarted);
	}
}

bool UMCRoleInputComponent::RouteInput(const FGameplayTag& InputTag) const
{
	return GameplayInputRouter.IsValid()
		&& GameplayInputRouter->RouteInput(InputTag);
}

void UMCRoleInputComponent::HandleBlockedInputTagsChanged(
	const FGameplayTagContainer& BlockedInputTags)
{
	if (IsInputTagBlocked(BlockedInputTags, MCGameplayTags::Input_Gameplay_Move)
		|| IsInputTagBlocked(BlockedInputTags, MCGameplayTags::Input_Gameplay_Move_WorldSpace))
	{
		InputState.MoveInput = FVector2D::ZeroVector;
		InputState.bMoveInputIsWorldSpace = false;
	}

	if (IsInputTagBlocked(BlockedInputTags, MCGameplayTags::Input_Gameplay_Look)
		|| IsInputTagBlocked(BlockedInputTags, MCGameplayTags::Input_Gameplay_Look_Mouse)
		|| IsInputTagBlocked(BlockedInputTags, MCGameplayTags::Input_Gameplay_Look_Gamepad))
	{
		InputState.LookInput = FVector2D::ZeroVector;
	}

	if (IsInputTagBlocked(BlockedInputTags, MCGameplayTags::Input_Gameplay_Jump))
	{
		InputState.bJumpPressed = false;
		InputState.bJumpJustPressed = false;
	}

	if (IsInputTagBlocked(BlockedInputTags, MCGameplayTags::Input_Gameplay_Crouch))
	{
		const bool bWasCrouchPressed = InputState.bCrouchPressed;
		InputState.bCrouchPressed = false;

		if (bWasCrouchPressed && IsValid(RoleMoverComponent))
		{
			RoleMoverComponent->UnCrouch();
		}
	}

	if (IsInputTagBlocked(BlockedInputTags, MCGameplayTags::Input_Gameplay_Sprint))
	{
		InputState.bSprintPressed = false;
	}

	if (IsInputTagBlocked(BlockedInputTags, MCGameplayTags::Input_Gameplay_Walk))
	{
		InputState.bWalkPressed = false;
	}

	if (IsInputTagBlocked(BlockedInputTags, MCGameplayTags::Input_Gameplay_Strafe))
	{
		InputState.bStrafePressed = false;
	}

	if (IsInputTagBlocked(BlockedInputTags, MCGameplayTags::Input_Gameplay_Aim))
	{
		InputState.bAimPressed = false;
	}

	if (IsInputTagBlocked(BlockedInputTags, MCGameplayTags::Input_Gameplay_Traverse))
	{
		InputState.bTraverseJustPressed = false;
	}

	if (IsInputTagBlocked(BlockedInputTags, MCGameplayTags::Input_Gameplay_Interact))
	{
		InputState.bInteractJustPressed = false;
	}
}

bool UMCRoleInputComponent::IsInputTagBlocked(
	const FGameplayTagContainer& BlockedInputTags,
	const FGameplayTag& InputTag)
{
	for (const FGameplayTag& BlockedInputTag : BlockedInputTags)
	{
		if (InputTag.MatchesTag(BlockedInputTag))
		{
			return true;
		}
	}

	return false;
}

void UMCRoleInputComponent::OnMoveTriggered(const FInputActionValue& Value)
{
	if (!RouteInput(MCGameplayTags::Input_Gameplay_Move))
	{
		return;
	}

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
	if (!RouteInput(MCGameplayTags::Input_Gameplay_Move_WorldSpace))
	{
		return;
	}

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
	if (!RouteInput(MCGameplayTags::Input_Gameplay_Look_Mouse))
	{
		return;
	}

	InputState.LookInput = Value.Get<FVector2D>();
	ApplyLookInput(InputState.LookInput, FVector2D::UnitVector);
}

void UMCRoleInputComponent::OnLookCompleted(const FInputActionValue& Value)
{
	InputState.LookInput = FVector2D::ZeroVector;
}

void UMCRoleInputComponent::OnLookGamepadTriggered(const FInputActionValue& Value)
{
	if (!RouteInput(MCGameplayTags::Input_Gameplay_Look_Gamepad))
	{
		return;
	}

	InputState.LookInput = Value.Get<FVector2D>();

	if (IsValid(InputConfig))
	{
		const UWorld* World = GetWorld();
		const float DeltaSeconds = IsValid(World) ? World->GetDeltaSeconds() : 0.0f;
		ApplyLookInput(InputState.LookInput, InputConfig->GamepadLookRate * DeltaSeconds);
	}
}

void UMCRoleInputComponent::OnLookGamepadCompleted(const FInputActionValue& Value)
{
	InputState.LookInput = FVector2D::ZeroVector;
}

void UMCRoleInputComponent::OnJumpStarted(const FInputActionValue& Value)
{
	if (!RouteInput(MCGameplayTags::Input_Gameplay_Jump))
	{
		return;
	}

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
	if (!RouteInput(MCGameplayTags::Input_Gameplay_Crouch))
	{
		return;
	}

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
	if (!RouteInput(MCGameplayTags::Input_Gameplay_Sprint))
	{
		return;
	}

	InputState.bSprintPressed = true;
}

void UMCRoleInputComponent::OnSprintCompleted(const FInputActionValue& Value)
{
	InputState.bSprintPressed = false;
}

void UMCRoleInputComponent::OnWalkStarted(const FInputActionValue& Value)
{
	if (!RouteInput(MCGameplayTags::Input_Gameplay_Walk))
	{
		return;
	}

	InputState.bWalkPressed = true;
}

void UMCRoleInputComponent::OnWalkCompleted(const FInputActionValue& Value)
{
	InputState.bWalkPressed = false;
}

void UMCRoleInputComponent::OnStrafeStarted(const FInputActionValue& Value)
{
	if (!RouteInput(MCGameplayTags::Input_Gameplay_Strafe))
	{
		return;
	}

	InputState.bStrafePressed = true;
}

void UMCRoleInputComponent::OnStrafeCompleted(const FInputActionValue& Value)
{
	InputState.bStrafePressed = false;
}

void UMCRoleInputComponent::OnAimStarted(const FInputActionValue& Value)
{
	if (!RouteInput(MCGameplayTags::Input_Gameplay_Aim))
	{
		return;
	}

	InputState.bAimPressed = true;
}

void UMCRoleInputComponent::OnAimCompleted(const FInputActionValue& Value)
{
	InputState.bAimPressed = false;
}

void UMCRoleInputComponent::OnTraverseStarted(const FInputActionValue& Value)
{
	if (!RouteInput(MCGameplayTags::Input_Gameplay_Traverse))
	{
		return;
	}

	InputState.bTraverseJustPressed = true;
	OnTraverseRequested.Broadcast();
}

void UMCRoleInputComponent::OnInteractStarted(const FInputActionValue& Value)
{
	if (!RouteInput(MCGameplayTags::Input_Gameplay_Interact))
	{
		return;
	}

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
