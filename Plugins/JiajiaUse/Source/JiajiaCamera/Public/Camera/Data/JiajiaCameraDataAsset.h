#pragma once

#include "Camera/JiajiaCameraTypes.h"
#include "Engine/DataAsset.h"
#include "JiajiaCameraDataAsset.generated.h"

UCLASS(BlueprintType, Blueprintable)
class JIAJIACAMERA_API UJiajiaCameraDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera Arm")
	FJiajiaCameraArmLengthSettings ArmLengthSettings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera Arm")
	FJiajiaCameraArmOffsetSettings ArmOffsetSettings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera Arm")
	FJiajiaCameraArmLagSettings ArmLagSettings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera FOV")
	FJiajiaCameraFOVSettings FOVSettings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera Pitch")
	FJiajiaCameraPitchToArmAndFOVSettings PitchToArmAndFOVSettings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera Collision")
	FJiajiaCameraCollisionSettings CollisionSettings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera Correction")
	FJiajiaCameraPitchCorrectionSettings PitchCorrectionSettings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera Correction")
	FJiajiaCameraYawCorrectionSettings YawCorrectionSettings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera Constraint")
	FJiajiaCameraPitchConstraintSettings PitchConstraintSettings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera Constraint")
	FJiajiaCameraYawConstraintSettings YawConstraintSettings;
};
