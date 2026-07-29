#pragma once

#include "CoreMinimal.h"
#include "JiajiaCameraTypes.generated.h"

class UCurveFloat;

USTRUCT(BlueprintType)
struct JIAJIACAMERA_API FJiajiaCameraPenetrationFeeler
{
	GENERATED_BODY()

	FJiajiaCameraPenetrationFeeler() = default;

	FJiajiaCameraPenetrationFeeler(
		const FRotator& InAdjustmentRotation,
		const float InWorldWeight,
		const float InPawnWeight,
		const float InProbeRadius)
		: AdjustmentRotation(InAdjustmentRotation)
		, WorldWeight(InWorldWeight)
		, PawnWeight(InPawnWeight)
		, ProbeRadius(InProbeRadius)
	{
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	FRotator AdjustmentRotation = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision",
		meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float WorldWeight = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision",
		meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float PawnWeight = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision", meta = (ClampMin = "0.0"))
	float ProbeRadius = 5.0f;
};

USTRUCT(BlueprintType)
struct JIAJIACAMERA_API FJiajiaCameraCollisionSettings
{
	GENERATED_BODY()

	FJiajiaCameraCollisionSettings();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision", meta = (ClampMin = "0.0"))
	float PenetrationBlendInTime = 0.05f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision", meta = (ClampMin = "0.0"))
	float PenetrationBlendOutTime = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	bool bPreventPenetration = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision",
		meta = (EditCondition = "bPreventPenetration"))
	bool bDoPredictiveAvoidance = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision",
		meta = (EditCondition = "bPreventPenetration && bDoPredictiveAvoidance"))
	TArray<FJiajiaCameraPenetrationFeeler> PenetrationAvoidanceFeelers;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
	FName IgnoreCameraCollisionTag = TEXT("JiajiaCamera_IgnoreCameraCollision");
};

USTRUCT(BlueprintType)
struct JIAJIACAMERA_API FJiajiaCameraArmLengthSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Arm")
	float MinArmLength = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Arm")
	float MaxArmLength = 250.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Arm", meta = (ClampMin = "0.0"))
	float ArmLengthBlendTime = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Arm")
	TObjectPtr<UCurveFloat> ArmLengthBlendCurve = nullptr;
};

USTRUCT(BlueprintType)
struct JIAJIACAMERA_API FJiajiaCameraFOVSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera FOV")
	float MinFOV = 90.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera FOV")
	float MaxFOV = 125.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera FOV", meta = (ClampMin = "0.0"))
	float FOVBlendTime = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera FOV")
	TObjectPtr<UCurveFloat> FOVBlendCurve = nullptr;
};

USTRUCT(BlueprintType)
struct JIAJIACAMERA_API FJiajiaCameraArmOffsetSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Arm")
	FVector SocketOffset = FVector(0.0f, 40.0f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Arm", meta = (ClampMin = "0.0"))
	float SocketOffsetBlendTime = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Arm")
	TObjectPtr<UCurveFloat> SocketOffsetBlendCurve = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Arm")
	FVector TargetOffset = FVector(0.0f, 0.0f, 58.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Arm", meta = (ClampMin = "0.0"))
	float TargetOffsetBlendTime = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Arm")
	TObjectPtr<UCurveFloat> TargetOffsetBlendCurve = nullptr;
};

USTRUCT(BlueprintType)
struct JIAJIACAMERA_API FJiajiaCameraArmLagSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Lag")
	bool bOverrideSpringArmSettings = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Lag",
		meta = (EditCondition = "bOverrideSpringArmSettings"))
	bool bEnableCameraLag = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Lag",
		meta = (EditCondition = "bOverrideSpringArmSettings"))
	bool bEnableCameraRotationLag = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Lag",
		meta = (EditCondition = "bOverrideSpringArmSettings && bEnableCameraLag", ClampMin = "0.0"))
	float CameraLagSpeed = 8.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Lag",
		meta = (EditCondition = "bOverrideSpringArmSettings && bEnableCameraRotationLag", ClampMin = "0.0"))
	float CameraRotationLagSpeed = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Lag",
		meta = (EditCondition = "bOverrideSpringArmSettings && bEnableCameraLag", ClampMin = "0.0"))
	float CameraLagMaxDistance = 0.0f;
};

USTRUCT(BlueprintType)
struct JIAJIACAMERA_API FJiajiaCameraPitchToArmAndFOVSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Pitch")
	bool bEnabled = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Pitch")
	TObjectPtr<UCurveFloat> PitchToArmLengthCurve = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Pitch")
	TObjectPtr<UCurveFloat> PitchToFOVCurve = nullptr;
};

USTRUCT(BlueprintType)
struct JIAJIACAMERA_API FJiajiaCameraPitchCorrectionSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pitch Correction")
	bool bEnabled = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pitch Correction", meta = (ClampMin = "0.0"))
	float CorrectionSpeed = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pitch Correction", meta = (ClampMin = "0.0"))
	float InputDelay = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pitch Correction")
	float RestingPitch = -10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pitch Correction", meta = (ClampMin = "0.0"))
	float AngleThreshold = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pitch Correction")
	bool bTriggerWhenFalling = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pitch Correction",
		meta = (EditCondition = "bTriggerWhenFalling", ClampMin = "0.0"))
	float FallingInputDelay = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pitch Correction",
		meta = (EditCondition = "bTriggerWhenFalling", ClampMin = "0.0"))
	float MinimumGroundDistanceWhenFalling = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pitch Correction",
		meta = (EditCondition = "bTriggerWhenFalling", ClampMin = "0.0"))
	float FallingSpeedMultiplier = 6.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pitch Correction")
	bool bTriggerOnSlopes = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pitch Correction",
		meta = (EditCondition = "bTriggerOnSlopes", ClampMin = "0.0"))
	float MinimumSlopeAngle = 25.0f;
};

USTRUCT(BlueprintType)
struct JIAJIACAMERA_API FJiajiaCameraYawCorrectionSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Yaw Correction")
	bool bEnabled = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Yaw Correction", meta = (ClampMin = "0.0"))
	float CorrectionSpeed = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Yaw Correction", meta = (ClampMin = "0.0"))
	float InputDelay = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Yaw Correction", meta = (ClampMin = "0.0"))
	float AngleThreshold = 10.0f;
};

USTRUCT(BlueprintType)
struct JIAJIACAMERA_API FJiajiaCameraPitchConstraintSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Constraint")
	bool bConstrainPitch = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Constraint")
	float LocalMinPitch = -89.9f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Constraint")
	float LocalMaxPitch = 89.9f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Constraint", meta = (ClampMin = "0.0"))
	float Tolerance = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Constraint", meta = (ClampMin = "0.0"))
	float BlendTime = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Constraint")
	TObjectPtr<UCurveFloat> BlendCurve = nullptr;
};

USTRUCT(BlueprintType)
struct JIAJIACAMERA_API FJiajiaCameraYawConstraintSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Constraint")
	bool bConstrainYaw = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Constraint")
	float LocalMinYaw = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Constraint")
	float LocalMaxYaw = 359.999f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Constraint", meta = (ClampMin = "0.0"))
	float Tolerance = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Constraint", meta = (ClampMin = "0.0"))
	float BlendTime = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Constraint")
	TObjectPtr<UCurveFloat> BlendCurve = nullptr;
};
