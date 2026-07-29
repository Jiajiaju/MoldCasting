UCLASS(Abstract)
class UMCASCrouchingArmAndFOVCameraModifier : UJiajiaBaseCameraModifier
{
	bool bCameraTransitionActive = false;
	bool bViewTargetIsOwner = true;
	bool bCrouching = false;
	bool bInitialized = false;
	float CrouchingBlendTimer = 0.0f;
	float StandingFOV = 90.0f;
	float StandingArmLength = 250.0f;
	float BlendStartFOV = 90.0f;
	float BlendStartArmLength = 250.0f;
	float TargetFOV = 90.0f;
	float TargetArmLength = 250.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bEnabled = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float CrouchingFOVMultiplier = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float CrouchingArmLengthMultiplier = 0.8f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float CrouchingBlendTime = 1.0f;

	UFUNCTION(BlueprintOverride)
	void OnModifierEnabled(FMinimalViewInfo LastPOV)
	{
		StandingFOV = LastPOV.FOV;
		StandingArmLength = JiajiaCameraManager.ASIsValid()
			? JiajiaCameraManager.GetOriginalArmLength()
			: CurrentArmLength;
		BlendStartFOV = StandingFOV;
		BlendStartArmLength = StandingArmLength;
		TargetFOV = StandingFOV;
		TargetArmLength = StandingArmLength;
		CrouchingBlendTimer = 0.0f;
		bCrouching = false;
		bInitialized = true;
	}

	UFUNCTION(BlueprintOverride)
	void OnModifierDisabled(FMinimalViewInfo LastPOV, bool bWasImmediate)
	{
		bInitialized = false;
		bCrouching = false;
		CrouchingBlendTimer = 0.0f;
	}

	UFUNCTION(BlueprintOverride)
	void OnCameraTransitionStarted()
	{
		bCameraTransitionActive = true;
	}

	UFUNCTION(BlueprintOverride)
	void OnCameraTransitionEnded()
	{
		bCameraTransitionActive = false;
		bInitialized = false;
	}

	UFUNCTION(BlueprintOverride)
	void OnViewTargetChanged(bool bImmediate, bool bNewTargetIsOwner)
	{
		bViewTargetIsOwner = bNewTargetIsOwner;
		bInitialized = false;
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
			|| !bEnabled
			|| bCameraTransitionActive
			|| !bViewTargetIsOwner)
		{
			return;
		}

		if (!bInitialized)
		{
			StandingFOV = InFOV;
			StandingArmLength = InArmLength;
			BlendStartFOV = InFOV;
			BlendStartArmLength = InArmLength;
			TargetFOV = InFOV;
			TargetArmLength = InArmLength;
			CrouchingBlendTimer = 0.0f;
			bCrouching = IsOwnerCrouching();
			bInitialized = true;

			if (bCrouching)
			{
				TargetFOV =
					StandingFOV * CrouchingFOVMultiplier;
				TargetArmLength =
					StandingArmLength * CrouchingArmLengthMultiplier;
			}
		}

		bool bOwnerCrouching = IsOwnerCrouching();

		if (bOwnerCrouching != bCrouching)
		{
			bCrouching = bOwnerCrouching;
			CrouchingBlendTimer = 0.0f;
			BlendStartFOV = InFOV;
			BlendStartArmLength = InArmLength;
			TargetFOV = bCrouching
				? StandingFOV * CrouchingFOVMultiplier
				: StandingFOV;
			TargetArmLength = bCrouching
				? StandingArmLength * CrouchingArmLengthMultiplier
				: StandingArmLength;
		}

		CrouchingBlendTimer = CrouchingBlendTimer + DeltaTime;
		float BlendAlpha = CrouchingBlendTime > 0.0f
			? Math::Clamp(
				CrouchingBlendTimer / CrouchingBlendTime,
				0.0f,
				1.0f)
			: 1.0f;

		OutFOV = float32(Math::Lerp(
			BlendStartFOV,
			TargetFOV,
			BlendAlpha));
		OutArmLength = float32(Math::Lerp(
			BlendStartArmLength,
			TargetArmLength,
			BlendAlpha));
	}
}
