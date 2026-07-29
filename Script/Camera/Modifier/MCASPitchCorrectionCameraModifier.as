UENUM()
enum EMCASPitchCorrectionMovementState
{
	None,
	Moving,
	Slope,
	Falling
}

UCLASS(Abstract)
class UMCASPitchCorrectionCameraModifier : UJiajiaBaseCameraModifier
{
	EMCASPitchCorrectionMovementState PitchCorrectionMovementState =
		EMCASPitchCorrectionMovementState::None;
	float32 CurrentSlopeAngle = 0.0f;
	float BlendTimer = 0.0f;
	float ThresholdTimer = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FJiajiaCameraPitchCorrectionSettings PitchCorrectionSettings;

	UFUNCTION(BlueprintOverride)
	bool ProcessControlRotation(
		AActor InViewTarget,
		float DeltaTime,
		FVector InViewLocation,
		FRotator InViewRotation,
		FRotator InLocalControlRotation,
		FRotator InDeltaRotation,
		FRotator& OutDeltaRotation)
	{
		OutDeltaRotation = InDeltaRotation;

		if (!OwnerPawn.ASIsValid()
			|| !JiajiaCameraManager.ASIsValid()
			|| !bHasVelocity
			|| !PitchCorrectionSettings.bEnabled)
		{
			ResetCorrection();
			return false;
		}

		PitchCorrectionMovementState =
			UpdatePitchCorrectionMovementState(InLocalControlRotation);
		UpdateThresholdTimer(DeltaTime);

		if (PitchCorrectionMovementState
				== EMCASPitchCorrectionMovementState::None
			|| bHasRotationInput
			|| ThresholdTimer <= GetInputDelay())
		{
			BlendTimer = 0.0f;
			return false;
		}

		BlendTimer = Math::Clamp(
			BlendTimer + DeltaTime,
			0.0f,
			AlphaInTime);

		FRotator TargetDeltaRotation(
			InDeltaRotation.Pitch
				+ GetCorrectionSpeed()
				* DeltaTime
				* GetCorrectionDirection(
					PitchCorrectionMovementState,
					InLocalControlRotation),
			InDeltaRotation.Yaw,
			InDeltaRotation.Roll);

		float BlendAlpha = AlphaInTime > 0.0f
			? BlendTimer / AlphaInTime
			: 1.0f;
		OutDeltaRotation = LerpRotationShortestPath(
			InDeltaRotation,
			TargetDeltaRotation,
			BlendAlpha);
		return false;
	}

	void ResetCorrection()
	{
		PitchCorrectionMovementState =
			EMCASPitchCorrectionMovementState::None;
		BlendTimer = 0.0f;
		ThresholdTimer = 0.0f;
	}

	void UpdateThresholdTimer(float DeltaTime)
	{
		bool bWaitingForMovement =
			PitchCorrectionMovementState
				!= EMCASPitchCorrectionMovementState::Falling
			&& !bHasMovementInput;

		if (bHasRotationInput || bWaitingForMovement)
		{
			ThresholdTimer = 0.0f;
			return;
		}

		ThresholdTimer = ThresholdTimer + DeltaTime;
	}

	EMCASPitchCorrectionMovementState UpdatePitchCorrectionMovementState(
		FRotator InLocalControlRotation)
	{
		float32 SlopeRollAngle = 0.0f;
		ComputeOwnerSlopeAngle(CurrentSlopeAngle, SlopeRollAngle);

		bool bSlope = PitchCorrectionSettings.bTriggerOnSlopes
			&& bHasMovementInput
			&& Math::Abs(CurrentSlopeAngle)
				>= PitchCorrectionSettings.MinimumSlopeAngle
			&& IsOwnerMovingOnGround()
			&& !Math::IsNearlyEqual(
				InLocalControlRotation.Pitch
					* Math::Sign(ComputeOwnerLookAndMovementDot()),
				CurrentSlopeAngle,
				2.0);

		if (bSlope)
		{
			return EMCASPitchCorrectionMovementState::Slope;
		}

		bool bMoving = bHasMovementInput
			&& IsOwnerMovingOnGround()
			&& !Math::IsNearlyEqual(
				InLocalControlRotation.Pitch,
				PitchCorrectionSettings.RestingPitch,
				PitchCorrectionSettings.AngleThreshold)
			&& (!PitchCorrectionSettings.bTriggerOnSlopes
				|| Math::IsNearlyEqual(
					CurrentSlopeAngle,
					0.0f,
					0.0001));

		if (bMoving)
		{
			return EMCASPitchCorrectionMovementState::Moving;
		}

		bool bFloorExists = false;
		float32 FloorDistance = 0.0f;
		ComputeOwnerFloorDistance(
			PitchCorrectionSettings.MinimumGroundDistanceWhenFalling,
			0.0f,
			bFloorExists,
			FloorDistance);

		bool bFalling = PitchCorrectionSettings.bTriggerWhenFalling
			&& IsOwnerFalling()
			&& InLocalControlRotation.Pitch
				>= JiajiaCameraManager.ViewPitchMin
			&& bFloorExists
			&& FloorDistance
				>= PitchCorrectionSettings.MinimumGroundDistanceWhenFalling;

		if (bFalling)
		{
			return EMCASPitchCorrectionMovementState::Falling;
		}

		return EMCASPitchCorrectionMovementState::None;
	}

	float GetCorrectionSpeed()
	{
		float FallingMultiplier = IsOwnerFalling()
			? PitchCorrectionSettings.FallingSpeedMultiplier
			: 1.0f;
		return PitchCorrectionSettings.CorrectionSpeed * FallingMultiplier;
	}

	float GetCorrectionDirection(
		EMCASPitchCorrectionMovementState InMovementState,
		FRotator InLocalControlRotation)
	{
		if (InMovementState == EMCASPitchCorrectionMovementState::Moving
			|| InMovementState == EMCASPitchCorrectionMovementState::Falling)
		{
			return PitchCorrectionSettings.RestingPitch
					> InLocalControlRotation.Pitch
				? 1.0f
				: -1.0f;
		}

		if (InMovementState == EMCASPitchCorrectionMovementState::Slope)
		{
			float LookMovementDotSign =
				Math::Sign(ComputeOwnerLookAndMovementDot());
			return CurrentSlopeAngle * LookMovementDotSign
					> InLocalControlRotation.Pitch
				? 1.0f
				: -1.0f;
		}

		return 0.0f;
	}

	float GetInputDelay()
	{
		return PitchCorrectionMovementState
				== EMCASPitchCorrectionMovementState::Falling
			? PitchCorrectionSettings.FallingInputDelay
			: PitchCorrectionSettings.InputDelay;
	}

	FRotator LerpRotationShortestPath(
		FRotator CurrentRotation,
		FRotator TargetRotation,
		float BlendAlpha)
	{
		FRotator DeltaRotation =
			(TargetRotation - CurrentRotation).GetNormalized();
		return CurrentRotation
			+ DeltaRotation * Math::Clamp(BlendAlpha, 0.0f, 1.0f);
	}
}
