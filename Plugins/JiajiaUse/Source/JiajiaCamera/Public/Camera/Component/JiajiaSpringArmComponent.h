#pragma once

#include "Camera/JiajiaCameraTypes.h"
#include "GameFramework/SpringArmComponent.h"
#include "JiajiaSpringArmComponent.generated.h"

UCLASS(Blueprintable, ClassGroup = Camera, meta = (BlueprintSpawnableComponent))
class JIAJIACAMERA_API UJiajiaSpringArmComponent : public USpringArmComponent
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Jiajia Camera|Lag")
	float GetHorizontalCameraLagSpeed() const;

	UFUNCTION(BlueprintCallable, Category = "Jiajia Camera|Lag")
	void SetHorizontalCameraLagSpeed(float InCameraLagSpeed);

	UFUNCTION(BlueprintPure, Category = "Jiajia Camera|Lag")
	float GetVerticalCameraLagSpeed() const;

	UFUNCTION(BlueprintCallable, Category = "Jiajia Camera|Lag")
	void SetVerticalCameraLagSpeed(float InCameraLagSpeed);

	UFUNCTION(BlueprintPure, Category = "Jiajia Camera|Collision")
	FJiajiaCameraCollisionSettings GetCameraCollisionSettings() const;

	UFUNCTION(BlueprintCallable, Category = "Jiajia Camera|Collision")
	void SetCameraCollisionSettings(const FJiajiaCameraCollisionSettings& InCollisionSettings);

	UFUNCTION(BlueprintCallable, Category = "Jiajia Camera|Collision")
	void ResetCollisionState();

protected:
	virtual void UpdateDesiredArmLocation(
		bool bDoTrace,
		bool bDoLocationLag,
		bool bDoRotationLag,
		float DeltaTime) override;

	virtual FVector BlendLocations(
		const FVector& DesiredArmLocation,
		const FVector& TraceHitLocation,
		bool bHitSomething,
		float DeltaTime) override;

	bool IsPlayerControlled() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lag",
		meta = (EditCondition = "bEnableCameraLag", ClampMin = "0.0"))
	float HorizontalCameraLagSpeed = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Lag",
		meta = (EditCondition = "bEnableCameraLag", ClampMin = "0.0"))
	float VerticalCameraLagSpeed = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Collision",
		meta = (EditCondition = "bDoCollisionTest"))
	FJiajiaCameraCollisionSettings CameraCollisionSettings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Collision",
		meta = (EditCondition = "bDoCollisionTest"))
	bool bMaintainFramingDuringCollisions = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera Collision")
	bool bDrawCollisionDebug = false;

private:
	float DistBlockedPct = 1.0f;
};
