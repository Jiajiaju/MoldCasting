#pragma once

#include "Camera/CameraModifier.h"
#include "JiajiaBaseCameraModifier.generated.h"

class AJiajiaCameraManager;
class APawn;
class APlayerController;
class USpringArmComponent;

UCLASS(Abstract, Blueprintable)
class JIAJIACAMERA_API UJiajiaBaseCameraModifier : public UCameraModifier
{
	GENERATED_BODY()

public:
	UJiajiaBaseCameraModifier();

	virtual void EnableModifier() override;
	virtual void DisableModifier(bool bImmediate = false) override;

	UFUNCTION(BlueprintNativeEvent, Category = "Jiajia Camera|Modifier")
	void OnModifierEnabled(const FMinimalViewInfo& LastPOV);

	UFUNCTION(BlueprintNativeEvent, Category = "Jiajia Camera|Modifier")
	void OnModifierDisabled(const FMinimalViewInfo& LastPOV, bool bWasImmediate);

	UFUNCTION(BlueprintNativeEvent, Category = "Jiajia Camera|Modifier")
	bool ProcessControlRotation(
		AActor* ViewTarget,
		float DeltaTime,
		FVector ViewLocation,
		FRotator ViewRotation,
		FRotator LocalControlRotation,
		FRotator InDeltaRotation,
		FRotator& OutDeltaRotation);

	UFUNCTION(BlueprintNativeEvent, Category = "Jiajia Camera|Modifier")
	void ProcessBoomLengthAndFOV(
		float DeltaTime,
		float InFOV,
		float InArmLength,
		FVector ViewLocation,
		FRotator ViewRotation,
		float& OutFOV,
		float& OutArmLength);

	UFUNCTION(BlueprintNativeEvent, Category = "Jiajia Camera|Modifier")
	void ProcessBoomOffsets(
		float DeltaTime,
		FVector InSocketOffset,
		FVector InTargetOffset,
		FVector ViewLocation,
		FRotator ViewRotation,
		FVector& OutSocketOffset,
		FVector& OutTargetOffset);

	UFUNCTION(BlueprintNativeEvent, Category = "Jiajia Camera|Modifier")
	void PostUpdate(float DeltaTime, FVector ViewLocation, FRotator ViewRotation);

	UFUNCTION(BlueprintNativeEvent, Category = "Jiajia Camera|Modifier")
	bool ProcessTurnRate(
		float DeltaTime,
		FRotator LocalControlRotation,
		float InPitchTurnRate,
		float InYawTurnRate,
		float& OutPitchTurnRate,
		float& OutYawTurnRate);

	UFUNCTION(BlueprintNativeEvent, Category = "Jiajia Camera|Modifier")
	void OnViewTargetChanged(bool bImmediate, bool bNewTargetIsOwner);

	UFUNCTION(BlueprintNativeEvent, Category = "Jiajia Camera|Modifier")
	void OnCameraTransitionStarted();

	UFUNCTION(BlueprintNativeEvent, Category = "Jiajia Camera|Modifier")
	void OnCameraTransitionEnded();

	UFUNCTION(BlueprintPure, Category = "Jiajia Camera|Modifier")
	bool CanRunDuringCameraAnimation() const;

	UFUNCTION(BlueprintPure, Category = "Jiajia Camera|Modifier")
	bool IsDebugEnabled() const;

	UFUNCTION(BlueprintCallable, Category = "Jiajia Camera|Modifier")
	void SetDebugEnabled(bool bEnabled);

	UFUNCTION(BlueprintCallable, Category = "Jiajia Camera|Modifier")
	void SetModifierAlpha(float InAlpha);

	UFUNCTION(BlueprintPure, Category = "Jiajia Camera|Modifier")
	AJiajiaCameraManager* GetJiajiaCameraManager() const;

	UFUNCTION(BlueprintPure, Category = "Jiajia Camera|Modifier")
	APawn* GetOwnerPawn() const;

	UFUNCTION(BlueprintPure, Category = "Jiajia Camera|Modifier")
	USpringArmComponent* GetSpringArmComponent() const;

	UFUNCTION(BlueprintPure, Category = "Jiajia Camera|Movement")
	FVector GetOwnerVelocity() const;

	UFUNCTION(BlueprintPure, Category = "Jiajia Camera|Movement")
	bool IsOwnerFalling() const;

	UFUNCTION(BlueprintPure, Category = "Jiajia Camera|Movement")
	bool IsOwnerStrafing() const;

	UFUNCTION(BlueprintPure, Category = "Jiajia Camera|Movement")
	bool IsOwnerMovingOnGround() const;

	UFUNCTION(BlueprintPure, Category = "Jiajia Camera|Movement")
	bool IsOwnerCrouching() const;

	UFUNCTION(BlueprintPure, Category = "Jiajia Camera|Movement")
	bool IsOwnerStanding() const;

	UFUNCTION(BlueprintPure, Category = "Jiajia Camera|Movement")
	void ComputeOwnerFloorDistance(
		float SweepDistance,
		float ProbeRadius,
		bool& bOutFloorExists,
		float& OutFloorDistance) const;

	UFUNCTION(BlueprintPure, Category = "Jiajia Camera|Movement")
	void ComputeOwnerFloorNormal(
		float SweepDistance,
		float ProbeRadius,
		bool& bOutFloorExists,
		FVector& OutFloorNormal) const;

	UFUNCTION(BlueprintPure, Category = "Jiajia Camera|Movement")
	void ComputeOwnerSlopeAngle(
		float& OutSlopePitchDegrees,
		float& OutSlopeRollDegrees) const;

	UFUNCTION(BlueprintPure, Category = "Jiajia Camera|Movement")
	float ComputeOwnerLookAndMovementDot() const;

protected:
	virtual void ModifyCamera(
		float DeltaTime,
		FVector ViewLocation,
		FRotator ViewRotation,
		float FOV,
		FVector& OutViewLocation,
		FRotator& OutViewRotation,
		float& OutFOV) override;

	virtual bool ProcessViewRotation(
		AActor* ViewTarget,
		float DeltaTime,
		FRotator& OutViewRotation,
		FRotator& OutDeltaRotation) override;

	void UpdateOwnerReferences();
	void UpdateInternalVariables();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Modifier Settings")
	bool bRunDuringCameraAnimations = false;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Internal")
	TObjectPtr<AJiajiaCameraManager> JiajiaCameraManager = nullptr;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Internal")
	TObjectPtr<APlayerController> OwnerController = nullptr;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Internal")
	TObjectPtr<APawn> OwnerPawn = nullptr;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Internal")
	TObjectPtr<USpringArmComponent> SpringArmComponent = nullptr;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Internal")
	FVector CurrentSocketOffset = FVector::ZeroVector;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Internal")
	FVector CurrentTargetOffset = FVector::ZeroVector;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Internal")
	float CurrentArmLength = 0.0f;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Internal")
	bool bHasVelocity = false;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Internal")
	bool bHasMovementInput = false;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Internal")
	FVector PreviousMovementInput = FVector::ZeroVector;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Internal")
	FVector MovementInput = FVector::ZeroVector;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Internal")
	float TimeSinceMovementInput = 0.0f;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Internal")
	bool bHasRotationInput = false;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Internal")
	FRotator RotationInput = FRotator::ZeroRotator;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Internal")
	float TimeSinceRotationInput = 0.0f;

private:
	bool bWarnedInvalidCameraManager = false;
	bool bWarnedMissingSpringArm = false;
};
