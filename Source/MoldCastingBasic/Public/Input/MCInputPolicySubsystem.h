#pragma once

#include "Input/MCInputTypes.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "MCInputPolicySubsystem.generated.h"

class UCommonUIActionRouterBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
	FMCInputPolicyChangedEvent,
	FGameplayTagContainer,
	BlockedInputTags,
	FGameplayTag,
	ResolvedPolicyTag);

DECLARE_MULTICAST_DELEGATE_TwoParams(
	FMCInputPolicyChangedNativeEvent,
	const FGameplayTagContainer&,
	const FGameplayTag&);

UCLASS()
class MOLDCASTINGBASIC_API UMCInputPolicySubsystem : public ULocalPlayerSubsystem
{
	GENERATED_BODY()

private:
	struct FMCActiveInputPolicy
	{
		FMCInputPolicyHandle Handle {};
		FMCInputPolicyDefinition Definition {};
		TWeakObjectPtr<const UObject> Source {};
		uint64 ActivationSequence = 0;
		bool bHasSource = false;
	};

	TMap<int32, FMCActiveInputPolicy> ActivePolicies {};
	FGameplayTagContainer BlockedInputTags {};
	TOptional<FMCInputPolicyDefinition> ResolvedPolicyDefinition {};
	int32 NextPolicyHandleId = 0;
	uint64 NextActivationSequence = 0;
	FMCInputPolicyChangedNativeEvent InputPolicyChangedNativeEvent {};

	UPROPERTY(Transient)
	TObjectPtr<UCommonUIActionRouterBase> CommonUIActionRouter = nullptr;

public:
	UPROPERTY(BlueprintAssignable, Category = "Input|Policy")
	FMCInputPolicyChangedEvent OnInputPolicyChangedEvent {};

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;

	virtual void PlayerControllerChanged(APlayerController* NewPlayerController) override;

	UFUNCTION(BlueprintCallable, Category = "Input|Policy")
	FMCInputPolicyHandle AcquirePolicy(FGameplayTag PolicyTag, UObject* Source);

	UFUNCTION(BlueprintCallable, Category = "Input|Policy")
	bool ReleasePolicy(FMCInputPolicyHandle Handle);

	UFUNCTION(BlueprintCallable, Category = "Input|Policy")
	void ReleasePoliciesForSource(const UObject* Source);

	UFUNCTION(BlueprintPure, Category = "Input|Policy")
	bool IsInputBlocked(FGameplayTag InputTag) const;

	UFUNCTION(BlueprintPure, Category = "Input|Policy")
	FGameplayTagContainer GetBlockedInputTags() const;

	UFUNCTION(BlueprintPure, Category = "Input|Policy")
	FGameplayTag GetResolvedPolicyTag() const;

	UFUNCTION(BlueprintPure, Category = "Input|Policy")
	bool GetResolvedPolicyDefinition(FMCInputPolicyDefinition& OutDefinition) const;

	UFUNCTION(BlueprintPure, Category = "Input|Policy")
	bool GetPolicyDefinition(FGameplayTag PolicyTag, FMCInputPolicyDefinition& OutDefinition) const;

	FMCInputPolicyChangedNativeEvent& OnInputPolicyChanged() { return InputPolicyChangedNativeEvent; }

private:
	void RebuildResolvedPolicy();

	void ApplyResolvedUIInputConfig();

	void FlushPressedKeys() const;
};
