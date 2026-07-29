#pragma once

#include "Camera/PlayerCameraManager.h"
#include "JiajiaCameraManager.generated.h"

class ACharacter;
class APawn;
class UCameraComponent;
class UCharacterMovementComponent;
class UCurveFloat;
class UJiajiaBaseCameraModifier;
class UJiajiaCameraDataAsset;
class USpringArmComponent;

UCLASS(Blueprintable)
class JIAJIACAMERA_API AJiajiaCameraManager : public APlayerCameraManager
{
	GENERATED_BODY()

public:
	AJiajiaCameraManager();

	virtual void InitializeFor(APlayerController* PlayerController) override;

	UFUNCTION(BlueprintPure, Category = "Jiajia Camera|View")
	void GetRealCameraView(FVector& OutViewLocation, FRotator& OutViewRotation) const;

	UFUNCTION(BlueprintPure, Category = "Jiajia Camera|Owner")
	APawn* GetOwnerPawn() const;

	UFUNCTION(BlueprintPure, Category = "Jiajia Camera|Owner")
	ACharacter* GetOwnerCharacter() const;

	UFUNCTION(BlueprintPure, Category = "Jiajia Camera|Owner")
	USpringArmComponent* GetOwnerSpringArmComponent() const;

	UFUNCTION(BlueprintCallable, Category = "Jiajia Camera|Owner")
	bool SetOwnerSpringArmComponent(USpringArmComponent* InSpringArmComponent);

	UFUNCTION(BlueprintPure, Category = "Jiajia Camera|State")
	FVector GetCachedOwnerVelocity() const;

	UFUNCTION(BlueprintPure, Category = "Jiajia Camera|State")
	bool HasVelocity() const;

	UFUNCTION(BlueprintPure, Category = "Jiajia Camera|State")
	FVector GetMovementInput() const;

	UFUNCTION(BlueprintPure, Category = "Jiajia Camera|State")
	bool HasMovementInput() const;

	UFUNCTION(BlueprintPure, Category = "Jiajia Camera|State")
	float GetTimeSinceMovementInput() const;

	UFUNCTION(BlueprintPure, Category = "Jiajia Camera|State")
	FRotator GetRotationInput() const;

	UFUNCTION(BlueprintPure, Category = "Jiajia Camera|State")
	bool HasRotationInput() const;

	UFUNCTION(BlueprintPure, Category = "Jiajia Camera|State")
	float GetTimeSinceRotationInput() const;

	UFUNCTION(BlueprintPure, Category = "Jiajia Camera|State")
	float GetOriginalArmLength() const;

	UFUNCTION(BlueprintPure, Category = "Jiajia Camera|State")
	float GetHorizontalFOV() const;

	UFUNCTION(BlueprintPure, Category = "Jiajia Camera|State")
	float GetVerticalFOV() const;

	UFUNCTION(BlueprintPure, Category = "Jiajia Camera|State")
	FVector2D GetArmLengthRange() const;

	UFUNCTION(BlueprintPure, Category = "Jiajia Camera|State")
	FVector2D GetFOVRange() const;

	UFUNCTION(BlueprintPure, Category = "Jiajia Camera|Input")
	FVector GetCameraTurnRate() const;

	UFUNCTION(BlueprintCallable, Category = "Jiajia Camera|Modifier")
	void SetJiajiaCameraModifiersEnabled(bool bEnabled, bool bImmediate = true);

	UFUNCTION(BlueprintCallable, Category = "Jiajia Camera|Modifier")
	void SetJiajiaCameraModifierDebugEnabled(bool bEnabled);

	UFUNCTION(BlueprintCallable, Category = "Jiajia Camera|Modifier")
	UCameraModifier* FindCameraModifierOfClass(
		TSubclassOf<UCameraModifier> ModifierClass,
		bool bIncludeDerivedClasses = true) const;

	template <typename ModifierType>
	ModifierType* FindCameraModifierOfType() const
	{
		return Cast<ModifierType>(
			FindCameraModifierOfClass(ModifierType::StaticClass(), true));
	}

	UFUNCTION(BlueprintPure, Category = "Jiajia Camera|Data")
	UJiajiaCameraDataAsset* GetCurrentCameraDataAsset() const;

	UFUNCTION(BlueprintCallable, Category = "Jiajia Camera|Data")
	bool PushCameraData(
		UJiajiaCameraDataAsset* CameraData,
		bool bBlendCameraProperties = true);

	UFUNCTION(BlueprintCallable, Category = "Jiajia Camera|Data")
	bool PopCurrentCameraData(bool bBlendCameraProperties = true);

	UFUNCTION(BlueprintCallable, Category = "Jiajia Camera|Data")
	bool PopCameraData(
		UJiajiaCameraDataAsset* CameraData,
		bool bBlendCameraProperties = true);

	UFUNCTION(BlueprintNativeEvent, Category = "Jiajia Camera|Data")
	void OnCameraDataStackChanged(
		UJiajiaCameraDataAsset* CameraData,
		bool bBlendCameraProperties);

	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "Jiajia Camera|Transition")
	bool IsCameraAnimationPlaying() const;

	UFUNCTION(BlueprintCallable, Category = "Jiajia Camera|Transition")
	void NotifyCameraTransitionStarted();

	UFUNCTION(BlueprintCallable, Category = "Jiajia Camera|Transition")
	void NotifyCameraTransitionEnded();

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
	virtual void Tick(float DeltaTime) override;
	virtual UCameraModifier* AddNewCameraModifier(
		TSubclassOf<UCameraModifier> ModifierClass) override;
	virtual bool RemoveCameraModifier(UCameraModifier* ModifierToRemove) override;
	virtual void SetViewTarget(
		AActor* NewViewTarget,
		FViewTargetTransitionParams TransitionParams) override;
	virtual void ProcessViewRotation(
		float DeltaTime,
		FRotator& OutViewRotation,
		FRotator& OutDeltaRotation) override;
	virtual void LimitViewYaw(
		FRotator& ViewRotation,
		float InViewYawMin,
		float InViewYawMax) override;

	UFUNCTION(BlueprintNativeEvent, Category = "Jiajia Camera|Internal")
	void UpdateInternalVariables(float DeltaTime);

	UFUNCTION(BlueprintNativeEvent, Category = "Jiajia Camera|Internal")
	USpringArmComponent* ResolveOwnerSpringArmComponent() const;

	UFUNCTION(BlueprintNativeEvent, Category = "Jiajia Camera|Internal")
	FRotator GetRotationControlInput() const;

	UFUNCTION(BlueprintNativeEvent, Category = "Jiajia Camera|Internal")
	FVector GetMovementControlInput() const;

	UFUNCTION(BlueprintNativeEvent, Category = "Jiajia Camera|Internal")
	FVector GetPawnVelocity() const;

	void ProcessTurnRate(
		float DeltaTime,
		const FRotator& LocalControlRotation,
		float& OutPitchTurnRate,
		float& OutYawTurnRate);

	void ForEachJiajiaCameraModifier(
		TFunctionRef<void(UJiajiaBaseCameraModifier*)> Function);

	void ForEachJiajiaCameraModifierUntil(
		TFunctionRef<bool(UJiajiaBaseCameraModifier*)> Function);

	void ClearOwnerState();
	void RefreshOwnerReferences();
	void ApplyCameraData(UJiajiaCameraDataAsset* CameraData, bool bBlendCameraProperties);
	void TickCameraDataBlend(float DeltaTime);
	UCameraComponent* FindOwnerCameraComponent() const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Jiajia Camera|Default")
	FVector2D ArmLengthRange = FVector2D(50.0f, 300.0f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Jiajia Camera|Default")
	FVector2D FOVRange = FVector2D(90.0f, 100.0f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Jiajia Camera|Input",
		meta = (ClampMin = "0.0"))
	float MovementInputThreshold = 0.1f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Jiajia Camera|Input",
		meta = (ClampMin = "0.0"))
	float RotationInputThreshold = 0.01f;

private:
	UPROPERTY(Transient)
	TObjectPtr<APawn> OwnerPawn = nullptr;

	UPROPERTY(Transient)
	TObjectPtr<ACharacter> OwnerCharacter = nullptr;

	UPROPERTY(Transient)
	TObjectPtr<UCharacterMovementComponent> CharacterMovementComponent = nullptr;

	UPROPERTY(Transient)
	TObjectPtr<USpringArmComponent> CameraSpringArmComponent = nullptr;

	UPROPERTY(Transient)
	TArray<TObjectPtr<UJiajiaBaseCameraModifier>> JiajiaCameraModifiers;

	UPROPERTY(Transient)
	TArray<TObjectPtr<UJiajiaCameraDataAsset>> CameraDataStack;

	FVector OwnerVelocity = FVector::ZeroVector;
	FVector MovementInput = FVector::ZeroVector;
	FRotator RotationInput = FRotator::ZeroRotator;

	float TimeSinceMovementInput = 0.0f;
	float TimeSinceRotationInput = 0.0f;
	float OriginalArmLength = 0.0f;
	float CachedAspectRatio = 16.0f / 9.0f;
	float HorizontalFOV = 90.0f;
	float VerticalFOV = 58.7155f;
	float PitchTurnRate = 1.0f;
	float YawTurnRate = 1.0f;

	bool bHasVelocity = false;
	bool bHasMovementInput = false;
	bool bHasRotationInput = false;

	struct FCameraDataBlendState
	{
		bool bActive = false;
		float ElapsedTime = 0.0f;
		float InitialArmLength = 0.0f;
		float TargetArmLength = 0.0f;
		float ArmLengthDuration = 0.0f;
		FVector InitialSocketOffset = FVector::ZeroVector;
		FVector TargetSocketOffset = FVector::ZeroVector;
		float SocketOffsetDuration = 0.0f;
		FVector InitialTargetOffset = FVector::ZeroVector;
		FVector TargetTargetOffset = FVector::ZeroVector;
		float TargetOffsetDuration = 0.0f;
		float InitialFOV = 90.0f;
		float TargetFOV = 90.0f;
		float FOVDuration = 0.0f;
		TWeakObjectPtr<UCurveFloat> ArmLengthCurve;
		TWeakObjectPtr<UCurveFloat> SocketOffsetCurve;
		TWeakObjectPtr<UCurveFloat> TargetOffsetCurve;
		TWeakObjectPtr<UCurveFloat> FOVCurve;
	};

	FCameraDataBlendState CameraDataBlendState;
};
