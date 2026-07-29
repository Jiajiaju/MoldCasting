UCLASS(Abstract)
class UMCASYawCorrectionCameraModifier : UJiajiaBaseCameraModifier
{
	float BlendTimer = 0.0f;
	float ThresholdTimer = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FJiajiaCameraYawCorrectionSettings YawCorrectionSettings;

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

		if (!OwnerPawn.ASIsValid() || !JiajiaCameraManager.ASIsValid())
		{
			ResetCorrection();
			return false;
		}

		UpdateThresholdTimer(DeltaTime);

		if (!CanCorrectYaw(InLocalControlRotation)
			|| bHasRotationInput
			|| ThresholdTimer <= YawCorrectionSettings.InputDelay)
		{
			BlendTimer = 0.0f;
			return false;
		}

		BlendTimer = Math::Clamp(
			BlendTimer + DeltaTime,
			0.0f,
			AlphaInTime);

		float LocalYaw = InLocalControlRotation.Yaw;
		float AngleScale = Math::Clamp(
			Math::Abs(LocalYaw) / 90.0f,
			0.001,
			1.0);
		FRotator TargetDeltaRotation(
			InDeltaRotation.Pitch,
			InDeltaRotation.Yaw
				- YawCorrectionSettings.CorrectionSpeed
				* DeltaTime
				* Math::Sign(LocalYaw)
				* AngleScale,
			InDeltaRotation.Roll);

		float BlendAlpha = AlphaInTime > 0.0f
			? BlendTimer / AlphaInTime
			: 1.0f;
		OutDeltaRotation = LerpRotationShortestPath(
			InDeltaRotation,
			TargetDeltaRotation,
			BlendAlpha);
		return true;
	}

	bool CanCorrectYaw(FRotator InLocalControlRotation)
	{
		if (!YawCorrectionSettings.bEnabled
			|| !bHasMovementInput
			|| !bHasVelocity
			|| IsOwnerStrafing())
		{
			return false;
		}

		FVector NormalizedMovementInput = MovementInput.GetSafeNormal();
		if (NormalizedMovementInput.GetAbs().Equals(
			FVector(1.0f, 0.0f, 0.0f),
			0.1f))
		{
			return false;
		}

		float AbsoluteLocalYaw = Math::Abs(InLocalControlRotation.Yaw);
		float AngleThreshold = YawCorrectionSettings.AngleThreshold;
		return AbsoluteLocalYaw >= AngleThreshold
			&& 180.0f - AbsoluteLocalYaw >= AngleThreshold;
	}

	void UpdateThresholdTimer(float DeltaTime)
	{
		if (bHasRotationInput || !bHasMovementInput)
		{
			ThresholdTimer = 0.0f;
			return;
		}

		ThresholdTimer = ThresholdTimer + DeltaTime;
	}

	void ResetCorrection()
	{
		BlendTimer = 0.0f;
		ThresholdTimer = 0.0f;
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
