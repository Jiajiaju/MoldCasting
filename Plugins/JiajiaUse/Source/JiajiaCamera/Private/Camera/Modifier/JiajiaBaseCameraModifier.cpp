#include "Camera/Modifier/JiajiaBaseCameraModifier.h"

#include "Camera/JiajiaCameraManager.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "JiajiaCamera.h"

UJiajiaBaseCameraModifier::UJiajiaBaseCameraModifier()
{
	Priority = 0;
}

void UJiajiaBaseCameraModifier::EnableModifier()
{
	Super::EnableModifier();

	if (CameraOwner)
	{
		OnModifierEnabled(CameraOwner->GetCameraCacheView());
	}
}

void UJiajiaBaseCameraModifier::DisableModifier(const bool bImmediate)
{
	Super::DisableModifier(bImmediate);

	if (bImmediate && bDisabled && !bPendingDisable)
	{
		Alpha = 0.0f;
	}

	if (CameraOwner)
	{
		OnModifierDisabled(CameraOwner->GetCameraCacheView(), bImmediate);
	}
}

void UJiajiaBaseCameraModifier::OnModifierEnabled_Implementation(
	const FMinimalViewInfo& LastPOV)
{
}

void UJiajiaBaseCameraModifier::OnModifierDisabled_Implementation(
	const FMinimalViewInfo& LastPOV,
	const bool bWasImmediate)
{
}

bool UJiajiaBaseCameraModifier::ProcessControlRotation_Implementation(
	AActor* ViewTarget,
	const float DeltaTime,
	const FVector ViewLocation,
	const FRotator ViewRotation,
	const FRotator LocalControlRotation,
	const FRotator InDeltaRotation,
	FRotator& OutDeltaRotation)
{
	OutDeltaRotation = InDeltaRotation;
	return false;
}

void UJiajiaBaseCameraModifier::ProcessBoomLengthAndFOV_Implementation(
	const float DeltaTime,
	const float InFOV,
	const float InArmLength,
	const FVector ViewLocation,
	const FRotator ViewRotation,
	float& OutFOV,
	float& OutArmLength)
{
	OutFOV = InFOV;
	OutArmLength = InArmLength;
}

void UJiajiaBaseCameraModifier::ProcessBoomOffsets_Implementation(
	const float DeltaTime,
	const FVector InSocketOffset,
	const FVector InTargetOffset,
	const FVector ViewLocation,
	const FRotator ViewRotation,
	FVector& OutSocketOffset,
	FVector& OutTargetOffset)
{
	OutSocketOffset = InSocketOffset;
	OutTargetOffset = InTargetOffset;
}

void UJiajiaBaseCameraModifier::PostUpdate_Implementation(
	const float DeltaTime,
	const FVector ViewLocation,
	const FRotator ViewRotation)
{
}

bool UJiajiaBaseCameraModifier::ProcessTurnRate_Implementation(
	const float DeltaTime,
	const FRotator LocalControlRotation,
	const float InPitchTurnRate,
	const float InYawTurnRate,
	float& OutPitchTurnRate,
	float& OutYawTurnRate)
{
	OutPitchTurnRate = InPitchTurnRate;
	OutYawTurnRate = InYawTurnRate;
	return false;
}

void UJiajiaBaseCameraModifier::OnViewTargetChanged_Implementation(
	const bool bImmediate,
	const bool bNewTargetIsOwner)
{
}

void UJiajiaBaseCameraModifier::OnCameraTransitionStarted_Implementation()
{
}

void UJiajiaBaseCameraModifier::OnCameraTransitionEnded_Implementation()
{
}

bool UJiajiaBaseCameraModifier::CanRunDuringCameraAnimation() const
{
	return bRunDuringCameraAnimations;
}

bool UJiajiaBaseCameraModifier::IsDebugEnabled() const
{
	return bDebug;
}

void UJiajiaBaseCameraModifier::SetDebugEnabled(const bool bEnabled)
{
	bDebug = bEnabled;
}

void UJiajiaBaseCameraModifier::SetModifierAlpha(const float InAlpha)
{
	Alpha = FMath::Clamp(InAlpha, 0.0f, 1.0f);
}

AJiajiaCameraManager* UJiajiaBaseCameraModifier::GetJiajiaCameraManager() const
{
	return JiajiaCameraManager;
}

APawn* UJiajiaBaseCameraModifier::GetOwnerPawn() const
{
	return OwnerPawn;
}

USpringArmComponent* UJiajiaBaseCameraModifier::GetSpringArmComponent() const
{
	return SpringArmComponent;
}

void UJiajiaBaseCameraModifier::ModifyCamera(
	const float DeltaTime,
	const FVector ViewLocation,
	const FRotator ViewRotation,
	const float FOV,
	FVector& OutViewLocation,
	FRotator& OutViewRotation,
	float& OutFOV)
{
	Super::ModifyCamera(
		DeltaTime,
		ViewLocation,
		ViewRotation,
		FOV,
		OutViewLocation,
		OutViewRotation,
		OutFOV);

	OutViewLocation = ViewLocation;
	OutViewRotation = ViewRotation;
	OutFOV = FOV;

	UpdateOwnerReferences();

	if (!JiajiaCameraManager || !OwnerController || !OwnerPawn)
	{
		return;
	}

	if (!bRunDuringCameraAnimations
		&& JiajiaCameraManager->IsCameraAnimationPlaying())
	{
		return;
	}

	UpdateInternalVariables();

	if (SpringArmComponent)
	{
		float NewFOV = FOV;
		float NewArmLength = CurrentArmLength;
		ProcessBoomLengthAndFOV(
			DeltaTime,
			FOV,
			CurrentArmLength,
			ViewLocation,
			ViewRotation,
			NewFOV,
			NewArmLength);
		OutFOV = NewFOV;
		SpringArmComponent->TargetArmLength = FMath::Max(NewArmLength, 0.0f);

		FVector NewSocketOffset = CurrentSocketOffset;
		FVector NewTargetOffset = CurrentTargetOffset;
		ProcessBoomOffsets(
			DeltaTime,
			CurrentSocketOffset,
			CurrentTargetOffset,
			ViewLocation,
			ViewRotation,
			NewSocketOffset,
			NewTargetOffset);
		SpringArmComponent->SocketOffset = NewSocketOffset;
		SpringArmComponent->TargetOffset = NewTargetOffset;
		bWarnedMissingSpringArm = false;
	}
	else if (!bWarnedMissingSpringArm)
	{
		UE_LOG(
			LogJiajiaCamera,
			Warning,
			TEXT("Camera modifier '%s' has no SpringArm on owner '%s'; boom hooks are skipped."),
			*GetNameSafe(this),
			*GetNameSafe(OwnerPawn));
		bWarnedMissingSpringArm = true;
	}

	PostUpdate(DeltaTime, ViewLocation, ViewRotation);
}

bool UJiajiaBaseCameraModifier::ProcessViewRotation(
	AActor* ViewTarget,
	const float DeltaTime,
	FRotator& OutViewRotation,
	FRotator& OutDeltaRotation)
{
	const bool bStopFromParent = Super::ProcessViewRotation(
		ViewTarget,
		DeltaTime,
		OutViewRotation,
		OutDeltaRotation);
	UpdateOwnerReferences();

	if (bStopFromParent
		|| !JiajiaCameraManager
		|| !CameraOwner
		|| !OwnerController)
	{
		return bStopFromParent;
	}

	if (!bRunDuringCameraAnimations
		&& JiajiaCameraManager->IsCameraAnimationPlaying())
	{
		return false;
	}

	const FRotator OwnerRotation = ViewTarget
		? ViewTarget->GetActorRotation()
		: FRotator::ZeroRotator;
	FRotator LocalControlRotation =
		OwnerController->GetControlRotation() - OwnerRotation;
	LocalControlRotation.Normalize();

	const FRotator InDeltaRotation = OutDeltaRotation;
	FRotator NewDeltaRotation = InDeltaRotation;
	const bool bStopProcessing = ProcessControlRotation(
		ViewTarget,
		DeltaTime,
		CameraOwner->GetCameraLocation(),
		OutViewRotation,
		LocalControlRotation,
		InDeltaRotation,
		NewDeltaRotation);
	OutDeltaRotation = NewDeltaRotation;
	return bStopProcessing;
}

void UJiajiaBaseCameraModifier::UpdateOwnerReferences()
{
	JiajiaCameraManager = Cast<AJiajiaCameraManager>(CameraOwner);

	if (!JiajiaCameraManager)
	{
		if (!bWarnedInvalidCameraManager)
		{
			UE_LOG(
				LogJiajiaCamera,
				Error,
				TEXT("Camera modifier '%s' requires AJiajiaCameraManager."),
				*GetNameSafe(this));
			bWarnedInvalidCameraManager = true;
		}

		OwnerController = nullptr;
		OwnerPawn = nullptr;
		SpringArmComponent = nullptr;
		return;
	}

	bWarnedInvalidCameraManager = false;
	OwnerController = JiajiaCameraManager->GetOwningPlayerController();
	OwnerPawn = JiajiaCameraManager->GetOwnerPawn();
	SpringArmComponent = JiajiaCameraManager->GetOwnerSpringArmComponent();
}

void UJiajiaBaseCameraModifier::UpdateInternalVariables()
{
	if (!JiajiaCameraManager)
	{
		return;
	}

	bHasVelocity = JiajiaCameraManager->HasVelocity();
	bHasMovementInput = JiajiaCameraManager->HasMovementInput();
	PreviousMovementInput = MovementInput;
	MovementInput = JiajiaCameraManager->GetMovementInput();
	TimeSinceMovementInput =
		JiajiaCameraManager->GetTimeSinceMovementInput();
	bHasRotationInput = JiajiaCameraManager->HasRotationInput();
	RotationInput = JiajiaCameraManager->GetRotationInput();
	TimeSinceRotationInput =
		JiajiaCameraManager->GetTimeSinceRotationInput();

	if (SpringArmComponent)
	{
		CurrentSocketOffset = SpringArmComponent->SocketOffset;
		CurrentTargetOffset = SpringArmComponent->TargetOffset;
		CurrentArmLength = SpringArmComponent->TargetArmLength;
	}
}

FVector UJiajiaBaseCameraModifier::GetOwnerVelocity() const
{
	return JiajiaCameraManager
		? JiajiaCameraManager->GetOwnerVelocity()
		: FVector::ZeroVector;
}

bool UJiajiaBaseCameraModifier::IsOwnerFalling() const
{
	return JiajiaCameraManager && JiajiaCameraManager->IsOwnerFalling();
}

bool UJiajiaBaseCameraModifier::IsOwnerStrafing() const
{
	return JiajiaCameraManager && JiajiaCameraManager->IsOwnerStrafing();
}

bool UJiajiaBaseCameraModifier::IsOwnerMovingOnGround() const
{
	return JiajiaCameraManager
		&& JiajiaCameraManager->IsOwnerMovingOnGround();
}

bool UJiajiaBaseCameraModifier::IsOwnerCrouching() const
{
	return JiajiaCameraManager && JiajiaCameraManager->IsOwnerCrouching();
}

bool UJiajiaBaseCameraModifier::IsOwnerStanding() const
{
	return JiajiaCameraManager && JiajiaCameraManager->IsOwnerStanding();
}

void UJiajiaBaseCameraModifier::ComputeOwnerFloorDistance(
	const float SweepDistance,
	const float ProbeRadius,
	bool& bOutFloorExists,
	float& OutFloorDistance) const
{
	bOutFloorExists = false;
	OutFloorDistance = 0.0f;

	if (JiajiaCameraManager)
	{
		JiajiaCameraManager->ComputeOwnerFloorDistance(
			SweepDistance,
			ProbeRadius,
			bOutFloorExists,
			OutFloorDistance);
	}
}

void UJiajiaBaseCameraModifier::ComputeOwnerFloorNormal(
	const float SweepDistance,
	const float ProbeRadius,
	bool& bOutFloorExists,
	FVector& OutFloorNormal) const
{
	bOutFloorExists = false;
	OutFloorNormal = FVector::ZeroVector;

	if (JiajiaCameraManager)
	{
		JiajiaCameraManager->ComputeOwnerFloorNormal(
			SweepDistance,
			ProbeRadius,
			bOutFloorExists,
			OutFloorNormal);
	}
}

void UJiajiaBaseCameraModifier::ComputeOwnerSlopeAngle(
	float& OutSlopePitchDegrees,
	float& OutSlopeRollDegrees) const
{
	OutSlopePitchDegrees = 0.0f;
	OutSlopeRollDegrees = 0.0f;

	if (JiajiaCameraManager)
	{
		JiajiaCameraManager->ComputeOwnerSlopeAngle(
			OutSlopePitchDegrees,
			OutSlopeRollDegrees);
	}
}

float UJiajiaBaseCameraModifier::ComputeOwnerLookAndMovementDot() const
{
	return JiajiaCameraManager
		? JiajiaCameraManager->ComputeOwnerLookAndMovementDot()
		: 0.0f;
}
