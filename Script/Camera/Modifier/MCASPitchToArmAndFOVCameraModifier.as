UCLASS(Abstract)
class UMCASPitchToArmAndFOVCameraModifier : UJiajiaBaseCameraModifier
{
	bool bCameraTransitionActive = false;
	bool bViewTargetIsOwner = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FJiajiaCameraPitchToArmAndFOVSettings PitchToArmAndFOVSettings;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	float PreEnabledFOV = 90.0f;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	float PreDisabledArmLength = 250.0f;

	UFUNCTION(BlueprintOverride)
	void OnModifierEnabled(FMinimalViewInfo LastPOV)
	{
		PreEnabledFOV = LastPOV.FOV;
	}

	UFUNCTION(BlueprintOverride)
	void OnModifierDisabled(FMinimalViewInfo LastPOV, bool bWasImmediate)
	{
		PreDisabledArmLength = CurrentArmLength;
	}

	UFUNCTION(BlueprintOverride)
	void OnCameraTransitionStarted()
	{
		bCameraTransitionActive = true;
		DisableModifier(true);
	}

	UFUNCTION(BlueprintOverride)
	void OnCameraTransitionEnded()
	{
		bCameraTransitionActive = false;

		if (bViewTargetIsOwner)
		{
			EnableModifier();
		}
	}

	UFUNCTION(BlueprintOverride)
	void OnViewTargetChanged(bool bImmediate, bool bNewTargetIsOwner)
	{
		bViewTargetIsOwner = bNewTargetIsOwner;

		if (!bNewTargetIsOwner)
		{
			DisableModifier(true);
			return;
		}

		if (!bCameraTransitionActive)
		{
			EnableModifier();
		}
	}

	UFUNCTION(BlueprintOverride)
	void ProcessBoomLengthAndFOV(
		float DeltaTime,
		float InFOV,
		float InArmLength,
		FVector ViewLocation,
		FRotator ViewRotation,
		float32& OutFOV,
		float32& OutArmLength)
	{
		OutFOV = float32(InFOV);
		OutArmLength = float32(InArmLength);

		if (!JiajiaCameraManager.ASIsValid()
			|| bCameraTransitionActive
			|| !bViewTargetIsOwner
			|| !PitchToArmAndFOVSettings.bEnabled
			|| !PitchToArmAndFOVSettings.PitchToFOVCurve.ASIsValid()
			|| !PitchToArmAndFOVSettings.PitchToArmLengthCurve.ASIsValid())
		{
			return;
		}

		float PitchRatio = Math::GetMappedRangeValueClamped(
			FVector2D(
				JiajiaCameraManager.ViewPitchMin,
				JiajiaCameraManager.ViewPitchMax),
			FVector2D(-1.0f, 1.0f),
			ViewRotation.Pitch);
		float PitchToFOVCurveValue =
			PitchToArmAndFOVSettings.PitchToFOVCurve.GetFloatValue(
				PitchRatio);
		float PitchToArmLengthCurveValue =
			PitchToArmAndFOVSettings.PitchToArmLengthCurve.GetFloatValue(
				PitchRatio);
		float PitchToFOV = Math::GetMappedRangeValueClamped(
			FVector2D(-1.0f, 1.0f),
			JiajiaCameraManager.GetFOVRange(),
			PitchToFOVCurveValue);
		float PitchToArmLength = Math::GetMappedRangeValueClamped(
			FVector2D(-1.0f, 1.0f),
			JiajiaCameraManager.GetArmLengthRange(),
			PitchToArmLengthCurveValue);

		OutFOV = float32(Math::Lerp(
			PreEnabledFOV,
			PitchToFOV,
			Alpha));
		OutArmLength = float32(Math::Lerp(
			PreDisabledArmLength,
			PitchToArmLength,
			Alpha));
	}
}
