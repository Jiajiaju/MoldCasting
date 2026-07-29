#include "Camera/Component/JiajiaSpringArmComponent.h"

#include "Camera/PlayerCameraManager.h"
#include "DrawDebugHelpers.h"
#include "Engine/Engine.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "JiajiaCamera.h"
#include "PhysicsEngine/PhysicsSettings.h"

namespace JiajiaCamera::SpringArm
{
	constexpr int32 MaxIgnoredActorsPerFeeler = 16;

	FVector InterpLocationByAxis(
		const FVector& Current,
		const FVector& Target,
		const float DeltaTime,
		const float HorizontalSpeed,
		const float VerticalSpeed)
	{
		return FVector(
			FMath::FInterpTo(Current.X, Target.X, DeltaTime, HorizontalSpeed),
			FMath::FInterpTo(Current.Y, Target.Y, DeltaTime, HorizontalSpeed),
			FMath::FInterpTo(Current.Z, Target.Z, DeltaTime, VerticalSpeed));
	}
}

float UJiajiaSpringArmComponent::GetHorizontalCameraLagSpeed() const
{
	return HorizontalCameraLagSpeed;
}

void UJiajiaSpringArmComponent::SetHorizontalCameraLagSpeed(const float InCameraLagSpeed)
{
	HorizontalCameraLagSpeed = FMath::Max(InCameraLagSpeed, 0.0f);
}

float UJiajiaSpringArmComponent::GetVerticalCameraLagSpeed() const
{
	return VerticalCameraLagSpeed;
}

void UJiajiaSpringArmComponent::SetVerticalCameraLagSpeed(const float InCameraLagSpeed)
{
	VerticalCameraLagSpeed = FMath::Max(InCameraLagSpeed, 0.0f);
}

FJiajiaCameraCollisionSettings UJiajiaSpringArmComponent::GetCameraCollisionSettings() const
{
	return CameraCollisionSettings;
}

void UJiajiaSpringArmComponent::SetCameraCollisionSettings(
	const FJiajiaCameraCollisionSettings& InCollisionSettings)
{
	CameraCollisionSettings = InCollisionSettings;
	ResetCollisionState();
}

void UJiajiaSpringArmComponent::ResetCollisionState()
{
	DistBlockedPct = 1.0f;
}

void UJiajiaSpringArmComponent::UpdateDesiredArmLocation(
	const bool bDoTrace,
	const bool bDoLocationLag,
	const bool bDoRotationLag,
	float DeltaTime)
{
	FRotator DesiredRotation = GetTargetRotation();

	if (bClampToMaxPhysicsDeltaTime)
	{
		DeltaTime = FMath::Min(DeltaTime, UPhysicsSettings::Get()->MaxPhysicsDeltaTime);
	}

	if (bDoRotationLag)
	{
		if (bUseCameraLagSubstepping
			&& DeltaTime > CameraLagMaxTimeStep
			&& CameraRotationLagSpeed > 0.0f)
		{
			const FRotator ArmRotationStep =
				(DesiredRotation - PreviousDesiredRot).GetNormalized() * (1.0f / DeltaTime);
			FRotator LerpTarget = PreviousDesiredRot;
			float RemainingTime = DeltaTime;

			while (RemainingTime > UE_KINDA_SMALL_NUMBER)
			{
				const float LerpAmount = FMath::Min(CameraLagMaxTimeStep, RemainingTime);
				LerpTarget += ArmRotationStep * LerpAmount;
				RemainingTime -= LerpAmount;

				DesiredRotation = FRotator(FMath::QInterpTo(
					FQuat(PreviousDesiredRot),
					FQuat(LerpTarget),
					LerpAmount,
					CameraRotationLagSpeed));
				PreviousDesiredRot = DesiredRotation;
			}
		}
		else
		{
			DesiredRotation = FRotator(FMath::QInterpTo(
				FQuat(PreviousDesiredRot),
				FQuat(DesiredRotation),
				DeltaTime,
				CameraRotationLagSpeed));
		}
	}

	PreviousDesiredRot = DesiredRotation;

	const FVector OriginalArmOrigin = GetComponentLocation() + TargetOffset;
	FVector ArmOrigin = OriginalArmOrigin;
	FVector DesiredLocation = ArmOrigin;

	if (bDoLocationLag)
	{
		const float EffectiveHorizontalSpeed =
			HorizontalCameraLagSpeed > 0.0f ? HorizontalCameraLagSpeed : CameraLagSpeed;
		const float EffectiveVerticalSpeed =
			VerticalCameraLagSpeed > 0.0f ? VerticalCameraLagSpeed : CameraLagSpeed;

		if (bUseCameraLagSubstepping
			&& DeltaTime > CameraLagMaxTimeStep
			&& (EffectiveHorizontalSpeed > 0.0f || EffectiveVerticalSpeed > 0.0f))
		{
			const FVector ArmMovementStep =
				(DesiredLocation - PreviousDesiredLoc) * (1.0f / DeltaTime);
			FVector LerpTarget = PreviousDesiredLoc;
			float RemainingTime = DeltaTime;

			while (RemainingTime > UE_KINDA_SMALL_NUMBER)
			{
				const float LerpAmount = FMath::Min(CameraLagMaxTimeStep, RemainingTime);
				LerpTarget += ArmMovementStep * LerpAmount;
				RemainingTime -= LerpAmount;

				DesiredLocation = JiajiaCamera::SpringArm::InterpLocationByAxis(
					PreviousDesiredLoc,
					LerpTarget,
					LerpAmount,
					EffectiveHorizontalSpeed,
					EffectiveVerticalSpeed);
				PreviousDesiredLoc = DesiredLocation;
			}
		}
		else
		{
			DesiredLocation = JiajiaCamera::SpringArm::InterpLocationByAxis(
				PreviousDesiredLoc,
				DesiredLocation,
				DeltaTime,
				EffectiveHorizontalSpeed,
				EffectiveVerticalSpeed);
		}

		bool bClampedDistance = false;

		if (CameraLagMaxDistance > 0.0f)
		{
			const FVector FromOrigin = DesiredLocation - ArmOrigin;

			if (FromOrigin.SizeSquared() > FMath::Square(CameraLagMaxDistance))
			{
				DesiredLocation =
					ArmOrigin + FromOrigin.GetClampedToMaxSize(CameraLagMaxDistance);
				bClampedDistance = true;
			}
		}

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
		if (bDrawDebugLagMarkers)
		{
			DrawDebugSphere(GetWorld(), ArmOrigin, 5.0f, 8, FColor::Green);
			DrawDebugSphere(GetWorld(), DesiredLocation, 5.0f, 8, FColor::Yellow);

			const FVector ToOrigin = ArmOrigin - DesiredLocation;
			DrawDebugDirectionalArrow(
				GetWorld(),
				DesiredLocation,
				DesiredLocation + ToOrigin * 0.5f,
				7.5f,
				bClampedDistance ? FColor::Red : FColor::Green);
			DrawDebugDirectionalArrow(
				GetWorld(),
				DesiredLocation + ToOrigin * 0.5f,
				ArmOrigin,
				7.5f,
				bClampedDistance ? FColor::Red : FColor::Green);
		}
#endif
	}

	PreviousArmOrigin = ArmOrigin;
	PreviousDesiredLoc = DesiredLocation;

	DesiredLocation -= DesiredRotation.Vector() * TargetArmLength;
	DesiredLocation += FRotationMatrix(DesiredRotation).TransformVector(SocketOffset);

	FVector ResultLocation;

	if (bDoTrace && TargetArmLength != 0.0f)
	{
		bIsCameraFixed = true;
		FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(JiajiaSpringArm), false, GetOwner());
		FHitResult Result;
		GetWorld()->SweepSingleByChannel(
			Result,
			OriginalArmOrigin,
			DesiredLocation,
			FQuat::Identity,
			ProbeChannel,
			FCollisionShape::MakeSphere(ProbeSize),
			QueryParams);

		UnfixedCameraPosition = DesiredLocation;
		ResultLocation = BlendLocations(
			DesiredLocation,
			Result.Location,
			Result.bBlockingHit,
			DeltaTime);

		if (ResultLocation == DesiredLocation)
		{
			bIsCameraFixed = false;
		}
	}
	else
	{
		ResultLocation = DesiredLocation;
		bIsCameraFixed = false;
		UnfixedCameraPosition = ResultLocation;
		ResetCollisionState();
	}

	const FTransform WorldCameraTransform(DesiredRotation, ResultLocation);
	const FTransform RelativeCameraTransform =
		WorldCameraTransform.GetRelativeTransform(GetComponentTransform());
	RelativeSocketLocation = RelativeCameraTransform.GetLocation();
	RelativeSocketRotation = RelativeCameraTransform.GetRotation();
	UpdateChildTransforms();
}

FVector UJiajiaSpringArmComponent::BlendLocations(
	const FVector& DesiredArmLocation,
	const FVector& TraceHitLocation,
	const bool bHitSomething,
	const float DeltaTime)
{
	if (!bDoCollisionTest || !CameraCollisionSettings.bPreventPenetration)
	{
		ResetCollisionState();
		return DesiredArmLocation;
	}

	const bool bShouldUsePredictiveAvoidance =
		CameraCollisionSettings.bDoPredictiveAvoidance
		&& !CameraCollisionSettings.PenetrationAvoidanceFeelers.IsEmpty()
		&& IsPlayerControlled();

	if (!bShouldUsePredictiveAvoidance)
	{
		ResetCollisionState();
		return Super::BlendLocations(
			DesiredArmLocation,
			TraceHitLocation,
			bHitSomething,
			DeltaTime);
	}

	const FRotator DesiredRotation = GetTargetRotation();
	const FVector SafeLocation = bMaintainFramingDuringCollisions
		? GetComponentLocation()
			+ TargetOffset
			+ FRotationMatrix(DesiredRotation).TransformVector(SocketOffset)
		: PreviousArmOrigin;
	const FVector BaseRay = DesiredArmLocation - SafeLocation;

	if (BaseRay.IsNearlyZero())
	{
		ResetCollisionState();
		return DesiredArmLocation;
	}

	UWorld* World = GetWorld();

	if (!IsValid(World))
	{
		return DesiredArmLocation;
	}

	const FRotationMatrix BaseMatrix(BaseRay.Rotation());
	const FVector Right = BaseMatrix.GetUnitAxis(EAxis::Y);
	const FVector Up = BaseMatrix.GetUnitAxis(EAxis::Z);

	float HardBlockedPct = 1.0f;
	float SoftBlockedPct = 1.0f;
	float BlockedThisFrame = 1.0f;
	int32 HitCount = 0;

	FCollisionQueryParams QueryParams(
		SCENE_QUERY_STAT(JiajiaCameraPenetration),
		false,
		GetOwner());
	QueryParams.AddIgnoredActor(GetOwner());

	const TArray<FJiajiaCameraPenetrationFeeler>& Feelers =
		CameraCollisionSettings.PenetrationAvoidanceFeelers;

	for (int32 FeelerIndex = 0; FeelerIndex < Feelers.Num(); ++FeelerIndex)
	{
		const FJiajiaCameraPenetrationFeeler& Feeler = Feelers[FeelerIndex];
		FRotator AdjustmentRotation = Feeler.AdjustmentRotation;

		if (FeelerIndex == 0 && !AdjustmentRotation.IsNearlyZero())
		{
			UE_LOG(
				LogJiajiaCamera,
				Error,
				TEXT("The first penetration feeler on '%s' must have zero adjustment rotation."),
				*GetNameSafe(this));
			AdjustmentRotation = FRotator::ZeroRotator;
		}

		const FVector RayTarget =
			BaseRay.RotateAngleAxis(AdjustmentRotation.Yaw, Up)
				.RotateAngleAxis(AdjustmentRotation.Pitch, Right)
			+ SafeLocation;
		const FCollisionShape Shape =
			FCollisionShape::MakeSphere(FMath::Max(Feeler.ProbeRadius, 0.0f));

		for (int32 Attempt = 0;
			Attempt < JiajiaCamera::SpringArm::MaxIgnoredActorsPerFeeler;
			++Attempt)
		{
			FHitResult Hit;
			const bool bHit = World->SweepSingleByChannel(
				Hit,
				SafeLocation,
				RayTarget,
				FQuat::Identity,
				ProbeChannel,
				Shape,
				QueryParams);

			if (!bHit)
			{
				break;
			}

			AActor* HitActor = Hit.GetActor();

			if (IsValid(HitActor)
				&& HitActor->ActorHasTag(CameraCollisionSettings.IgnoreCameraCollisionTag))
			{
				QueryParams.AddIgnoredActor(HitActor);
				continue;
			}

			++HitCount;
			const float Weight = HitActor && HitActor->IsA<APawn>()
				? Feeler.PawnWeight
				: Feeler.WorldWeight;
			const float NewBlockPct = FMath::Clamp(
				Hit.Time + (1.0f - Hit.Time) * (1.0f - Weight),
				0.0f,
				1.0f);
			BlockedThisFrame = FMath::Min(BlockedThisFrame, NewBlockPct);

			if (FeelerIndex == 0)
			{
				HardBlockedPct = BlockedThisFrame;
			}
			else
			{
				SoftBlockedPct = FMath::Min(SoftBlockedPct, BlockedThisFrame);
			}

			break;
		}
	}

#if ENABLE_DRAW_DEBUG
	if (bDrawCollisionDebug && HitCount > 0)
	{
		UE_LOG(
			LogJiajiaCamera,
			Verbose,
			TEXT("%s has %d colliding camera feelers."),
			*GetNameSafe(this),
			HitCount);
	}
#endif

	if (DistBlockedPct < BlockedThisFrame)
	{
		const float BlendOutTime = FMath::Max(
			CameraCollisionSettings.PenetrationBlendOutTime,
			UE_KINDA_SMALL_NUMBER);
		DistBlockedPct +=
			DeltaTime / BlendOutTime * (BlockedThisFrame - DistBlockedPct);
	}
	else if (DistBlockedPct > HardBlockedPct)
	{
		DistBlockedPct = HardBlockedPct;
	}
	else if (DistBlockedPct > SoftBlockedPct)
	{
		const float BlendInTime = FMath::Max(
			CameraCollisionSettings.PenetrationBlendInTime,
			UE_KINDA_SMALL_NUMBER);
		DistBlockedPct -=
			DeltaTime / BlendInTime * (DistBlockedPct - SoftBlockedPct);
	}

	DistBlockedPct = FMath::Clamp(DistBlockedPct, 0.0f, 1.0f);
	return SafeLocation + (DesiredArmLocation - SafeLocation) * DistBlockedPct;
}

bool UJiajiaSpringArmComponent::IsPlayerControlled() const
{
	const APawn* PawnOwner = Cast<APawn>(GetOwner());
	const AController* Controller = PawnOwner ? PawnOwner->GetController() : nullptr;

	if (PawnOwner && PawnOwner->IsPlayerControlled())
	{
		return true;
	}

	UWorld* World = GetWorld();
	APlayerController* PlayerController = GEngine && World
		? GEngine->GetFirstLocalPlayerController(World)
		: nullptr;
	APlayerCameraManager* CameraManager = PlayerController
		? PlayerController->PlayerCameraManager
		: nullptr;

	return CameraManager
		&& (CameraManager->PendingViewTarget.Target == GetOwner()
			|| (Controller && CameraManager->PendingViewTarget.Target == Controller));
}
