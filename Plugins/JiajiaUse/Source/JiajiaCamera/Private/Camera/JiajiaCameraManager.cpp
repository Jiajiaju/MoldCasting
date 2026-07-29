#include "Camera/JiajiaCameraManager.h"

#include "Camera/CameraComponent.h"
#include "Camera/Component/JiajiaSpringArmComponent.h"
#include "Camera/Data/JiajiaCameraDataAsset.h"
#include "Camera/Modifier/JiajiaBaseCameraModifier.h"
#include "Components/CapsuleComponent.h"
#include "Curves/CurveFloat.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Interface/JiajiaCameraInputInterface.h"
#include "Interface/JiajiaCameraMovementInterface.h"
#include "Kismet/KismetMathLibrary.h"

namespace JiajiaCamera::Manager
{
	constexpr float VelocitySquaredThreshold = 0.01f;
	constexpr float DefaultAspectRatio = 16.0f / 9.0f;
	constexpr float DefaultFloorSweepDistance = 96.0f;
	constexpr float DefaultFloorProbeRadius = 64.0f;

	float EvaluateBlend(
		const float ElapsedTime,
		const float Duration,
		const TWeakObjectPtr<UCurveFloat>& Curve)
	{
		if (Duration <= UE_KINDA_SMALL_NUMBER)
		{
			return 1.0f;
		}

		const float LinearAlpha = FMath::Clamp(ElapsedTime / Duration, 0.0f, 1.0f);
		return Curve.IsValid()
			? FMath::Clamp(Curve->GetFloatValue(LinearAlpha), 0.0f, 1.0f)
			: LinearAlpha;
	}
}

AJiajiaCameraManager::AJiajiaCameraManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AJiajiaCameraManager::InitializeFor(APlayerController* PlayerController)
{
	Super::InitializeFor(PlayerController);
	UpdateInternalVariables(0.0f);
}

void AJiajiaCameraManager::GetRealCameraView(
	FVector& OutViewLocation,
	FRotator& OutViewRotation) const
{
	OutViewLocation = ViewTarget.POV.Location;
	OutViewRotation = ViewTarget.POV.Rotation;
}

APawn* AJiajiaCameraManager::GetOwnerPawn() const
{
	return OwnerPawn;
}

ACharacter* AJiajiaCameraManager::GetOwnerCharacter() const
{
	return OwnerCharacter;
}

USpringArmComponent* AJiajiaCameraManager::GetOwnerSpringArmComponent() const
{
	return CameraSpringArmComponent;
}

bool AJiajiaCameraManager::SetOwnerSpringArmComponent(
	USpringArmComponent* InSpringArmComponent)
{
	if (!IsValid(InSpringArmComponent)
		|| !IsValid(OwnerPawn)
		|| InSpringArmComponent->GetOwner() != OwnerPawn)
	{
		return false;
	}

	CameraSpringArmComponent = InSpringArmComponent;
	OriginalArmLength = CameraSpringArmComponent->TargetArmLength;
	return true;
}

FVector AJiajiaCameraManager::GetCachedOwnerVelocity() const
{
	return OwnerVelocity;
}

bool AJiajiaCameraManager::HasVelocity() const
{
	return bHasVelocity;
}

FVector AJiajiaCameraManager::GetMovementInput() const
{
	return MovementInput;
}

bool AJiajiaCameraManager::HasMovementInput() const
{
	return bHasMovementInput;
}

float AJiajiaCameraManager::GetTimeSinceMovementInput() const
{
	return TimeSinceMovementInput;
}

FRotator AJiajiaCameraManager::GetRotationInput() const
{
	return RotationInput;
}

bool AJiajiaCameraManager::HasRotationInput() const
{
	return bHasRotationInput;
}

float AJiajiaCameraManager::GetTimeSinceRotationInput() const
{
	return TimeSinceRotationInput;
}

float AJiajiaCameraManager::GetOriginalArmLength() const
{
	return OriginalArmLength;
}

float AJiajiaCameraManager::GetHorizontalFOV() const
{
	return HorizontalFOV;
}

float AJiajiaCameraManager::GetVerticalFOV() const
{
	return VerticalFOV;
}

FVector2D AJiajiaCameraManager::GetArmLengthRange() const
{
	return ArmLengthRange;
}

FVector2D AJiajiaCameraManager::GetFOVRange() const
{
	return FOVRange;
}

FVector AJiajiaCameraManager::GetCameraTurnRate() const
{
	return FVector(YawTurnRate, PitchTurnRate, 0.0f);
}

void AJiajiaCameraManager::SetJiajiaCameraModifiersEnabled(
	const bool bEnabled,
	const bool bImmediate)
{
	ForEachJiajiaCameraModifier(
		[bEnabled, bImmediate](UJiajiaBaseCameraModifier* Modifier)
		{
			if (bEnabled)
			{
				Modifier->EnableModifier();
			}
			else
			{
				Modifier->DisableModifier(bImmediate);
			}
		});
}

void AJiajiaCameraManager::SetJiajiaCameraModifierDebugEnabled(const bool bEnabled)
{
	ForEachJiajiaCameraModifier(
		[bEnabled](UJiajiaBaseCameraModifier* Modifier)
		{
			Modifier->SetDebugEnabled(bEnabled);
		});
}

UCameraModifier* AJiajiaCameraManager::FindCameraModifierOfClass(
	const TSubclassOf<UCameraModifier> ModifierClass,
	const bool bIncludeDerivedClasses) const
{
	if (!ModifierClass)
	{
		return nullptr;
	}

	for (UCameraModifier* Modifier : ModifierList)
	{
		if (!IsValid(Modifier))
		{
			continue;
		}

		const bool bMatches = bIncludeDerivedClasses
			? Modifier->GetClass()->IsChildOf(ModifierClass)
			: Modifier->GetClass() == ModifierClass;

		if (bMatches)
		{
			return Modifier;
		}
	}

	return nullptr;
}

UJiajiaCameraDataAsset* AJiajiaCameraManager::GetCurrentCameraDataAsset() const
{
	return CameraDataStack.IsEmpty() ? nullptr : CameraDataStack.Last();
}

bool AJiajiaCameraManager::PushCameraData(
	UJiajiaCameraDataAsset* CameraData,
	const bool bBlendCameraProperties)
{
	if (!IsValid(CameraData))
	{
		return false;
	}

	CameraDataStack.Add(CameraData);
	OnCameraDataStackChanged(CameraData, bBlendCameraProperties);
	return true;
}

bool AJiajiaCameraManager::PopCurrentCameraData(const bool bBlendCameraProperties)
{
	if (CameraDataStack.IsEmpty())
	{
		return false;
	}

	CameraDataStack.Pop();
	OnCameraDataStackChanged(GetCurrentCameraDataAsset(), bBlendCameraProperties);
	return true;
}

bool AJiajiaCameraManager::PopCameraData(
	UJiajiaCameraDataAsset* CameraData,
	const bool bBlendCameraProperties)
{
	if (!IsValid(CameraData) || CameraDataStack.IsEmpty())
	{
		return false;
	}

	const bool bWasCurrent = GetCurrentCameraDataAsset() == CameraData;
	const int32 RemovedCount = CameraDataStack.Remove(CameraData);

	if (RemovedCount > 0 && bWasCurrent)
	{
		OnCameraDataStackChanged(GetCurrentCameraDataAsset(), bBlendCameraProperties);
	}

	return RemovedCount > 0;
}

void AJiajiaCameraManager::OnCameraDataStackChanged_Implementation(
	UJiajiaCameraDataAsset* CameraData,
	const bool bBlendCameraProperties)
{
	ApplyCameraData(CameraData, bBlendCameraProperties);
}

bool AJiajiaCameraManager::IsCameraAnimationPlaying_Implementation() const
{
	return false;
}

void AJiajiaCameraManager::NotifyCameraTransitionStarted()
{
	ForEachJiajiaCameraModifier(
		[](UJiajiaBaseCameraModifier* Modifier)
		{
			Modifier->OnCameraTransitionStarted();
		});
}

void AJiajiaCameraManager::NotifyCameraTransitionEnded()
{
	ForEachJiajiaCameraModifier(
		[](UJiajiaBaseCameraModifier* Modifier)
		{
			Modifier->OnCameraTransitionEnded();
		});
}

void AJiajiaCameraManager::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateInternalVariables(DeltaTime);
	TickCameraDataBlend(DeltaTime);
}

UCameraModifier* AJiajiaCameraManager::AddNewCameraModifier(
	const TSubclassOf<UCameraModifier> ModifierClass)
{
	UCameraModifier* AddedModifier = Super::AddNewCameraModifier(ModifierClass);

	if (UJiajiaBaseCameraModifier* JiajiaModifier =
		Cast<UJiajiaBaseCameraModifier>(AddedModifier))
	{
		JiajiaCameraModifiers.AddUnique(JiajiaModifier);
	}

	return AddedModifier;
}

bool AJiajiaCameraManager::RemoveCameraModifier(UCameraModifier* ModifierToRemove)
{
	if (UJiajiaBaseCameraModifier* JiajiaModifier =
		Cast<UJiajiaBaseCameraModifier>(ModifierToRemove))
	{
		JiajiaCameraModifiers.Remove(JiajiaModifier);
	}

	return Super::RemoveCameraModifier(ModifierToRemove);
}

void AJiajiaCameraManager::SetViewTarget(
	AActor* NewViewTarget,
	FViewTargetTransitionParams TransitionParams)
{
	AActor* PreviousTarget = ViewTarget.Target;
	AActor* PreviousPendingTarget = PendingViewTarget.Target;
	Super::SetViewTarget(NewViewTarget, TransitionParams);

	const bool bTargetChanged = PreviousTarget != ViewTarget.Target;
	const bool bPendingTargetChanged =
		PreviousPendingTarget != PendingViewTarget.Target;

	if (!bTargetChanged && !bPendingTargetChanged)
	{
		return;
	}

	const bool bImmediate = bTargetChanged && !bPendingTargetChanged;
	const AActor* EffectiveTarget = bImmediate
		? ViewTarget.Target.Get()
		: PendingViewTarget.Target.Get();
	const APawn* CurrentPawn = PCOwner ? PCOwner->GetPawn() : nullptr;
	const bool bNewTargetIsOwner =
		EffectiveTarget == CurrentPawn || EffectiveTarget == PCOwner.Get();

	ForEachJiajiaCameraModifier(
		[bImmediate, bNewTargetIsOwner](UJiajiaBaseCameraModifier* Modifier)
		{
			Modifier->OnViewTargetChanged(bImmediate, bNewTargetIsOwner);
		});
}

void AJiajiaCameraManager::ProcessViewRotation(
	const float DeltaTime,
	FRotator& OutViewRotation,
	FRotator& OutDeltaRotation)
{
	Super::ProcessViewRotation(DeltaTime, OutViewRotation, OutDeltaRotation);

	if (!PCOwner || !ViewTarget.Target)
	{
		PitchTurnRate = 1.0f;
		YawTurnRate = 1.0f;
		return;
	}

	FRotator LocalControlRotation =
		PCOwner->GetControlRotation() - ViewTarget.Target->GetActorRotation();
	LocalControlRotation.Normalize();

	float NewPitchTurnRate = PitchTurnRate;
	float NewYawTurnRate = YawTurnRate;
	ProcessTurnRate(
		DeltaTime,
		LocalControlRotation,
		NewPitchTurnRate,
		NewYawTurnRate);
	PitchTurnRate = FMath::Clamp(NewPitchTurnRate, 0.0f, 1.0f);
	YawTurnRate = FMath::Clamp(NewYawTurnRate, 0.0f, 1.0f);
}

void AJiajiaCameraManager::LimitViewYaw(
	FRotator& ViewRotation,
	const float InViewYawMin,
	const float InViewYawMax)
{
	const APawn* CurrentPawn = PCOwner ? PCOwner->GetPawn() : nullptr;

	if (!CurrentPawn)
	{
		Super::LimitViewYaw(ViewRotation, InViewYawMin, InViewYawMax);
		return;
	}

	const float ActorYaw = CurrentPawn->GetActorRotation().Yaw;
	ViewRotation.Yaw = FMath::ClampAngle(
		ViewRotation.Yaw,
		ActorYaw + InViewYawMin,
		ActorYaw + InViewYawMax);
	ViewRotation.Yaw = FRotator::ClampAxis(ViewRotation.Yaw);
}

void AJiajiaCameraManager::UpdateInternalVariables_Implementation(const float DeltaTime)
{
	CachedAspectRatio = GetCameraCacheView().AspectRatio;

	if (FMath::IsNearlyZero(CachedAspectRatio))
	{
		CachedAspectRatio = JiajiaCamera::Manager::DefaultAspectRatio;
	}

	HorizontalFOV = GetFOVAngle();
	VerticalFOV = FMath::RadiansToDegrees(
		2.0f * FMath::Atan(
			FMath::Tan(FMath::DegreesToRadians(HorizontalFOV) * 0.5f)
			/ CachedAspectRatio));

	RefreshOwnerReferences();

	if (!OwnerPawn)
	{
		return;
	}

	OwnerVelocity = GetPawnVelocity();
	bHasVelocity =
		OwnerVelocity.SizeSquared() >= JiajiaCamera::Manager::VelocitySquaredThreshold;

	MovementInput = GetMovementControlInput();
	bHasMovementInput =
		FMath::Abs(MovementInput.X) > MovementInputThreshold
		|| FMath::Abs(MovementInput.Y) > MovementInputThreshold
		|| FMath::Abs(MovementInput.Z) > MovementInputThreshold;
	TimeSinceMovementInput = bHasMovementInput
		? 0.0f
		: TimeSinceMovementInput + DeltaTime;

	RotationInput = GetRotationControlInput();
	bHasRotationInput =
		FMath::Abs(RotationInput.Pitch) > RotationInputThreshold
		|| FMath::Abs(RotationInput.Yaw) > RotationInputThreshold;
	TimeSinceRotationInput = bHasRotationInput
		? 0.0f
		: TimeSinceRotationInput + DeltaTime;
}

USpringArmComponent* AJiajiaCameraManager::ResolveOwnerSpringArmComponent_Implementation() const
{
	return OwnerPawn
		? OwnerPawn->FindComponentByClass<USpringArmComponent>()
		: nullptr;
}

FRotator AJiajiaCameraManager::GetRotationControlInput_Implementation() const
{
	const IJiajiaCameraInputInterface* InputInterface =
		Cast<IJiajiaCameraInputInterface>(OwnerPawn);

	if (!InputInterface)
	{
		return FRotator::ZeroRotator;
	}

	const FVector2D LookInput = InputInterface->GetCameraLookInput();
	return FRotator(LookInput.Y, LookInput.X, 0.0f);
}

FVector AJiajiaCameraManager::GetMovementControlInput_Implementation() const
{
	const IJiajiaCameraInputInterface* InputInterface =
		Cast<IJiajiaCameraInputInterface>(OwnerPawn);
	return InputInterface
		? InputInterface->GetCameraMoveInput()
		: FVector::ZeroVector;
}

FVector AJiajiaCameraManager::GetPawnVelocity_Implementation() const
{
	return GetOwnerVelocity();
}

void AJiajiaCameraManager::ProcessTurnRate(
	const float DeltaTime,
	const FRotator& LocalControlRotation,
	float& OutPitchTurnRate,
	float& OutYawTurnRate)
{
	ForEachJiajiaCameraModifierUntil(
		[&](UJiajiaBaseCameraModifier* Modifier)
		{
			if (Modifier->IsDisabled())
			{
				return false;
			}

			if (!Modifier->CanRunDuringCameraAnimation()
				&& IsCameraAnimationPlaying())
			{
				return false;
			}

			float ModifierPitchTurnRate = OutPitchTurnRate;
			float ModifierYawTurnRate = OutYawTurnRate;
			const bool bStopProcessing = Modifier->ProcessTurnRate(
				DeltaTime,
				LocalControlRotation,
				OutPitchTurnRate,
				OutYawTurnRate,
				ModifierPitchTurnRate,
				ModifierYawTurnRate);
			OutPitchTurnRate = ModifierPitchTurnRate;
			OutYawTurnRate = ModifierYawTurnRate;
			return bStopProcessing;
		});
}

void AJiajiaCameraManager::ForEachJiajiaCameraModifier(
	TFunctionRef<void(UJiajiaBaseCameraModifier*)> Function)
{
	JiajiaCameraModifiers.RemoveAll(
		[](const TObjectPtr<UJiajiaBaseCameraModifier>& Modifier)
		{
			return !IsValid(Modifier);
		});

	for (UJiajiaBaseCameraModifier* Modifier : JiajiaCameraModifiers)
	{
		Function(Modifier);
	}
}

void AJiajiaCameraManager::ForEachJiajiaCameraModifierUntil(
	TFunctionRef<bool(UJiajiaBaseCameraModifier*)> Function)
{
	JiajiaCameraModifiers.RemoveAll(
		[](const TObjectPtr<UJiajiaBaseCameraModifier>& Modifier)
		{
			return !IsValid(Modifier);
		});

	for (UJiajiaBaseCameraModifier* Modifier : JiajiaCameraModifiers)
	{
		if (Function(Modifier))
		{
			break;
		}
	}
}

void AJiajiaCameraManager::ClearOwnerState()
{
	OwnerPawn = nullptr;
	OwnerCharacter = nullptr;
	CharacterMovementComponent = nullptr;
	CameraSpringArmComponent = nullptr;
	OwnerVelocity = FVector::ZeroVector;
	MovementInput = FVector::ZeroVector;
	RotationInput = FRotator::ZeroRotator;
	bHasVelocity = false;
	bHasMovementInput = false;
	bHasRotationInput = false;
	TimeSinceMovementInput = 0.0f;
	TimeSinceRotationInput = 0.0f;
	OriginalArmLength = 0.0f;
	CameraDataBlendState = FCameraDataBlendState();
}

void AJiajiaCameraManager::RefreshOwnerReferences()
{
	APawn* NewOwnerPawn = PCOwner ? PCOwner->GetPawn() : nullptr;

	if (!NewOwnerPawn)
	{
		if (OwnerPawn)
		{
			ClearOwnerState();
		}

		return;
	}

	if (NewOwnerPawn != OwnerPawn)
	{
		OwnerPawn = NewOwnerPawn;
		OwnerCharacter = Cast<ACharacter>(OwnerPawn);
		CharacterMovementComponent = OwnerCharacter
			? OwnerCharacter->GetCharacterMovement()
			: nullptr;
		CameraSpringArmComponent = ResolveOwnerSpringArmComponent();
		OriginalArmLength = CameraSpringArmComponent
			? CameraSpringArmComponent->TargetArmLength
			: 0.0f;
		CameraDataBlendState = FCameraDataBlendState();
	}
	else if (!IsValid(CameraSpringArmComponent))
	{
		CameraSpringArmComponent = ResolveOwnerSpringArmComponent();
		OriginalArmLength = CameraSpringArmComponent
			? CameraSpringArmComponent->TargetArmLength
			: 0.0f;
	}
}

void AJiajiaCameraManager::ApplyCameraData(
	UJiajiaCameraDataAsset* CameraData,
	const bool bBlendCameraProperties)
{
	CameraDataBlendState = FCameraDataBlendState();

	if (!IsValid(CameraData) || !IsValid(CameraSpringArmComponent))
	{
		return;
	}

	if (UJiajiaSpringArmComponent* JiajiaSpringArm =
		Cast<UJiajiaSpringArmComponent>(CameraSpringArmComponent))
	{
		JiajiaSpringArm->SetCameraCollisionSettings(CameraData->CollisionSettings);
	}

	const FJiajiaCameraArmLagSettings& LagSettings = CameraData->ArmLagSettings;

	if (LagSettings.bOverrideSpringArmSettings)
	{
		CameraSpringArmComponent->bEnableCameraLag = LagSettings.bEnableCameraLag;
		CameraSpringArmComponent->bEnableCameraRotationLag =
			LagSettings.bEnableCameraRotationLag;
		CameraSpringArmComponent->CameraLagSpeed = LagSettings.CameraLagSpeed;
		CameraSpringArmComponent->CameraRotationLagSpeed =
			LagSettings.CameraRotationLagSpeed;
		CameraSpringArmComponent->CameraLagMaxDistance =
			LagSettings.CameraLagMaxDistance;
	}

	UCameraComponent* CameraComponent = FindOwnerCameraComponent();

	if (!bBlendCameraProperties)
	{
		CameraSpringArmComponent->TargetArmLength =
			CameraData->ArmLengthSettings.MinArmLength;
		CameraSpringArmComponent->SocketOffset =
			CameraData->ArmOffsetSettings.SocketOffset;
		CameraSpringArmComponent->TargetOffset =
			CameraData->ArmOffsetSettings.TargetOffset;

		if (CameraComponent)
		{
			CameraComponent->SetFieldOfView(CameraData->FOVSettings.MinFOV);
		}

		return;
	}

	CameraDataBlendState.bActive = true;
	CameraDataBlendState.InitialArmLength =
		CameraSpringArmComponent->TargetArmLength;
	CameraDataBlendState.TargetArmLength =
		CameraData->ArmLengthSettings.MinArmLength;
	CameraDataBlendState.ArmLengthDuration =
		CameraData->ArmLengthSettings.ArmLengthBlendTime;
	CameraDataBlendState.ArmLengthCurve =
		CameraData->ArmLengthSettings.ArmLengthBlendCurve;
	CameraDataBlendState.InitialSocketOffset =
		CameraSpringArmComponent->SocketOffset;
	CameraDataBlendState.TargetSocketOffset =
		CameraData->ArmOffsetSettings.SocketOffset;
	CameraDataBlendState.SocketOffsetDuration =
		CameraData->ArmOffsetSettings.SocketOffsetBlendTime;
	CameraDataBlendState.SocketOffsetCurve =
		CameraData->ArmOffsetSettings.SocketOffsetBlendCurve;
	CameraDataBlendState.InitialTargetOffset =
		CameraSpringArmComponent->TargetOffset;
	CameraDataBlendState.TargetTargetOffset =
		CameraData->ArmOffsetSettings.TargetOffset;
	CameraDataBlendState.TargetOffsetDuration =
		CameraData->ArmOffsetSettings.TargetOffsetBlendTime;
	CameraDataBlendState.TargetOffsetCurve =
		CameraData->ArmOffsetSettings.TargetOffsetBlendCurve;
	CameraDataBlendState.InitialFOV = CameraComponent
		? CameraComponent->FieldOfView
		: GetFOVAngle();
	CameraDataBlendState.TargetFOV = CameraData->FOVSettings.MinFOV;
	CameraDataBlendState.FOVDuration = CameraData->FOVSettings.FOVBlendTime;
	CameraDataBlendState.FOVCurve = CameraData->FOVSettings.FOVBlendCurve;
	TickCameraDataBlend(0.0f);
}

void AJiajiaCameraManager::TickCameraDataBlend(const float DeltaTime)
{
	if (!CameraDataBlendState.bActive || !IsValid(CameraSpringArmComponent))
	{
		return;
	}

	CameraDataBlendState.ElapsedTime += DeltaTime;

	const float ArmLengthAlpha = JiajiaCamera::Manager::EvaluateBlend(
		CameraDataBlendState.ElapsedTime,
		CameraDataBlendState.ArmLengthDuration,
		CameraDataBlendState.ArmLengthCurve);
	const float SocketOffsetAlpha = JiajiaCamera::Manager::EvaluateBlend(
		CameraDataBlendState.ElapsedTime,
		CameraDataBlendState.SocketOffsetDuration,
		CameraDataBlendState.SocketOffsetCurve);
	const float TargetOffsetAlpha = JiajiaCamera::Manager::EvaluateBlend(
		CameraDataBlendState.ElapsedTime,
		CameraDataBlendState.TargetOffsetDuration,
		CameraDataBlendState.TargetOffsetCurve);
	const float FOVAlpha = JiajiaCamera::Manager::EvaluateBlend(
		CameraDataBlendState.ElapsedTime,
		CameraDataBlendState.FOVDuration,
		CameraDataBlendState.FOVCurve);

	CameraSpringArmComponent->TargetArmLength = FMath::Lerp(
		CameraDataBlendState.InitialArmLength,
		CameraDataBlendState.TargetArmLength,
		ArmLengthAlpha);
	CameraSpringArmComponent->SocketOffset = FMath::Lerp(
		CameraDataBlendState.InitialSocketOffset,
		CameraDataBlendState.TargetSocketOffset,
		SocketOffsetAlpha);
	CameraSpringArmComponent->TargetOffset = FMath::Lerp(
		CameraDataBlendState.InitialTargetOffset,
		CameraDataBlendState.TargetTargetOffset,
		TargetOffsetAlpha);

	if (UCameraComponent* CameraComponent = FindOwnerCameraComponent())
	{
		CameraComponent->SetFieldOfView(FMath::Lerp(
			CameraDataBlendState.InitialFOV,
			CameraDataBlendState.TargetFOV,
			FOVAlpha));
	}

	CameraDataBlendState.bActive =
		ArmLengthAlpha < 1.0f
		|| SocketOffsetAlpha < 1.0f
		|| TargetOffsetAlpha < 1.0f
		|| FOVAlpha < 1.0f;
}

UCameraComponent* AJiajiaCameraManager::FindOwnerCameraComponent() const
{
	if (!CameraSpringArmComponent)
	{
		return nullptr;
	}

	for (int32 ChildIndex = 0;
		ChildIndex < CameraSpringArmComponent->GetNumChildrenComponents();
		++ChildIndex)
	{
		if (UCameraComponent* CameraComponent =
			Cast<UCameraComponent>(
				CameraSpringArmComponent->GetChildComponent(ChildIndex)))
		{
			return CameraComponent;
		}
	}

	return nullptr;
}

FVector AJiajiaCameraManager::GetOwnerVelocity() const
{
	if (CharacterMovementComponent)
	{
		return CharacterMovementComponent->Velocity;
	}

	const IJiajiaCameraMovementInterface* MovementInterface =
		Cast<IJiajiaCameraMovementInterface>(OwnerPawn);

	if (MovementInterface)
	{
		return MovementInterface->GetCameraVelocity();
	}

	return OwnerPawn ? OwnerPawn->GetVelocity() : FVector::ZeroVector;
}

bool AJiajiaCameraManager::IsOwnerFalling() const
{
	if (CharacterMovementComponent)
	{
		return CharacterMovementComponent->IsFalling();
	}

	const IJiajiaCameraMovementInterface* MovementInterface =
		Cast<IJiajiaCameraMovementInterface>(OwnerPawn);
	return MovementInterface && MovementInterface->IsCameraFalling();
}

bool AJiajiaCameraManager::IsOwnerStrafing() const
{
	if (CharacterMovementComponent && OwnerPawn)
	{
		return OwnerPawn->bUseControllerRotationYaw
			|| (CharacterMovementComponent->bUseControllerDesiredRotation
				&& !CharacterMovementComponent->bOrientRotationToMovement);
	}

	const IJiajiaCameraMovementInterface* MovementInterface =
		Cast<IJiajiaCameraMovementInterface>(OwnerPawn);
	return MovementInterface
		? MovementInterface->IsCameraStrafing()
		: OwnerPawn && OwnerPawn->bUseControllerRotationYaw;
}

bool AJiajiaCameraManager::IsOwnerMovingOnGround() const
{
	if (CharacterMovementComponent)
	{
		return CharacterMovementComponent->IsMovingOnGround();
	}

	const IJiajiaCameraMovementInterface* MovementInterface =
		Cast<IJiajiaCameraMovementInterface>(OwnerPawn);
	return MovementInterface && MovementInterface->IsCameraOnGround();
}

bool AJiajiaCameraManager::IsOwnerCrouching() const
{
	if (CharacterMovementComponent)
	{
		return CharacterMovementComponent->IsCrouching();
	}

	const IJiajiaCameraMovementInterface* MovementInterface =
		Cast<IJiajiaCameraMovementInterface>(OwnerPawn);
	return MovementInterface && MovementInterface->IsCameraCrouching();
}

bool AJiajiaCameraManager::IsOwnerStanding() const
{
	const IJiajiaCameraMovementInterface* MovementInterface =
		Cast<IJiajiaCameraMovementInterface>(OwnerPawn);

	if (MovementInterface)
	{
		return MovementInterface->IsCameraStanding();
	}

	return IsOwnerMovingOnGround() && !IsOwnerCrouching();
}

void AJiajiaCameraManager::ComputeOwnerFloorDistance(
	const float SweepDistance,
	float ProbeRadius,
	bool& bOutFloorExists,
	float& OutFloorDistance) const
{
	bOutFloorExists = false;
	OutFloorDistance = 0.0f;

	if (!OwnerPawn)
	{
		return;
	}

	if (CharacterMovementComponent && OwnerCharacter)
	{
		ProbeRadius = FMath::Max(
			ProbeRadius,
			OwnerCharacter->GetCapsuleComponent()->GetScaledCapsuleRadius());
		FFindFloorResult FloorResult;
		CharacterMovementComponent->ComputeFloorDist(
			OwnerPawn->GetActorLocation(),
			SweepDistance,
			SweepDistance,
			FloorResult,
			ProbeRadius);
		bOutFloorExists = FloorResult.bBlockingHit;
		OutFloorDistance = bOutFloorExists ? FloorResult.FloorDist : 0.0f;
		return;
	}

	UWorld* World = GetWorld();

	if (!World)
	{
		return;
	}

	const FVector Start = OwnerPawn->GetActorLocation();
	const FVector End =
		Start - OwnerPawn->GetActorUpVector() * FMath::Max(SweepDistance, 0.0f);
	FCollisionQueryParams QueryParams(
		SCENE_QUERY_STAT(JiajiaCameraFloorDistance),
		false,
		OwnerPawn);
	FHitResult Hit;
	bOutFloorExists = World->SweepSingleByChannel(
		Hit,
		Start,
		End,
		FQuat::Identity,
		ECC_Visibility,
		FCollisionShape::MakeSphere(FMath::Max(ProbeRadius, 0.0f)),
		QueryParams);
	OutFloorDistance = bOutFloorExists ? Hit.Distance : 0.0f;
}

void AJiajiaCameraManager::ComputeOwnerFloorNormal(
	const float SweepDistance,
	const float ProbeRadius,
	bool& bOutFloorExists,
	FVector& OutFloorNormal) const
{
	bOutFloorExists = false;
	OutFloorNormal = FVector::ZeroVector;

	if (!OwnerPawn)
	{
		return;
	}

	if (CharacterMovementComponent)
	{
		bOutFloorExists = CharacterMovementComponent->CurrentFloor.IsWalkableFloor();
		OutFloorNormal = bOutFloorExists
			? CharacterMovementComponent->CurrentFloor.HitResult.ImpactNormal
			: FVector::ZeroVector;
		return;
	}

	UWorld* World = GetWorld();

	if (!World)
	{
		return;
	}

	const FVector Start = OwnerPawn->GetActorLocation();
	const FVector End =
		Start - OwnerPawn->GetActorUpVector() * FMath::Max(SweepDistance, 0.0f);
	FCollisionQueryParams QueryParams(
		SCENE_QUERY_STAT(JiajiaCameraFloorNormal),
		false,
		OwnerPawn);
	FHitResult Hit;
	bOutFloorExists = World->SweepSingleByChannel(
		Hit,
		Start,
		End,
		FQuat::Identity,
		ECC_Visibility,
		FCollisionShape::MakeSphere(FMath::Max(ProbeRadius, 0.0f)),
		QueryParams);
	OutFloorNormal = bOutFloorExists ? Hit.ImpactNormal : FVector::ZeroVector;
}

void AJiajiaCameraManager::ComputeOwnerSlopeAngle(
	float& OutSlopePitchDegrees,
	float& OutSlopeRollDegrees) const
{
	OutSlopePitchDegrees = 0.0f;
	OutSlopeRollDegrees = 0.0f;

	if (!OwnerPawn)
	{
		return;
	}

	bool bFloorExists = false;
	FVector FloorNormal = FVector::ZeroVector;
	ComputeOwnerFloorNormal(
		JiajiaCamera::Manager::DefaultFloorSweepDistance,
		JiajiaCamera::Manager::DefaultFloorProbeRadius,
		bFloorExists,
		FloorNormal);

	if (bFloorExists)
	{
		UKismetMathLibrary::GetSlopeDegreeAngles(
			OwnerPawn->GetActorRightVector(),
			FloorNormal,
			OwnerPawn->GetActorUpVector(),
			OutSlopePitchDegrees,
			OutSlopeRollDegrees);
	}
}

float AJiajiaCameraManager::ComputeOwnerLookAndMovementDot() const
{
	if (IsOwnerStrafing())
	{
		return 1.0f;
	}

	if (!OwnerPawn)
	{
		return 0.0f;
	}

	const FVector VelocityDirection = GetOwnerVelocity().GetSafeNormal();

	if (VelocityDirection.IsNearlyZero())
	{
		return 0.0f;
	}

	return FVector::DotProduct(
		VelocityDirection,
		OwnerPawn->GetControlRotation().Vector().GetSafeNormal());
}
