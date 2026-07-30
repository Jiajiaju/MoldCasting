#pragma once

#include "CommonInputModeTypes.h"
#include "Engine/EngineBaseTypes.h"
#include "GameplayTagContainer.h"
#include "MCInputTypes.generated.h"

class UInputAction;

namespace MCInputMappingPriority
{
	inline constexpr int32 Gameplay = 0;
	inline constexpr int32 UI = 1000;
	inline constexpr int32 System = 2000;
}

USTRUCT(BlueprintType)
struct MOLDCASTINGBASIC_API FMCInputActionDefinition
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> InputAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (Categories = "Input.Gameplay"))
	FGameplayTag InputTag {};
};

USTRUCT(BlueprintType)
struct MOLDCASTINGBASIC_API FMCInputPolicyDefinition
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (Categories = "Input.Policy"))
	FGameplayTag PolicyTag {};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	int32 Priority = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	ECommonInputMode InputMode = ECommonInputMode::Game;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	EMouseCaptureMode MouseCaptureMode = EMouseCaptureMode::CapturePermanently_IncludingInitialMouseDown;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	EMouseLockMode MouseLockMode = EMouseLockMode::LockOnCapture;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	bool bHideCursorDuringViewportCapture = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (Categories = "Input.Gameplay"))
	FGameplayTagContainer BlockedInputTags {};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	bool bFlushPressedKeysOnActivation = true;

	bool BlocksInputTag(const FGameplayTag& InputTag) const;
};

USTRUCT(BlueprintType)
struct MOLDCASTINGBASIC_API FMCInputPolicyHandle
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, Category = "Input")
	int32 Id = INDEX_NONE;

	friend class UMCInputPolicySubsystem;

public:
	bool IsValid() const { return Id != INDEX_NONE; }

	void Reset() { Id = INDEX_NONE; }

	bool operator==(const FMCInputPolicyHandle& Other) const { return Id == Other.Id; }
};
