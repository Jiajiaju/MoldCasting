#pragma once

#include "GameplayTagContainer.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "MCGameplayInputRouter.generated.h"

class UMCInputPolicySubsystem;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FMCBlockedGameplayInputTagsChangedEvent,
	FGameplayTagContainer,
	BlockedInputTags);

DECLARE_MULTICAST_DELEGATE_OneParam(
	FMCBlockedGameplayInputTagsChangedNativeEvent,
	const FGameplayTagContainer&);

UCLASS()
class MOLDCASTINGBASIC_API UMCGameplayInputRouter : public ULocalPlayerSubsystem
{
	GENERATED_BODY()

private:
	UPROPERTY(Transient)
	TObjectPtr<UMCInputPolicySubsystem> InputPolicySubsystem = nullptr;

	FMCBlockedGameplayInputTagsChangedNativeEvent BlockedInputTagsChangedNativeEvent {};

public:
	UPROPERTY(BlueprintAssignable, Category = "Input|Gameplay")
	FMCBlockedGameplayInputTagsChangedEvent OnBlockedInputTagsChangedEvent {};

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void Deinitialize() override;

	UFUNCTION(BlueprintPure, Category = "Input|Gameplay")
	bool CanRouteInput(FGameplayTag InputTag) const;

	UFUNCTION(BlueprintCallable, Category = "Input|Gameplay")
	bool RouteInput(FGameplayTag InputTag) const;

	UFUNCTION(BlueprintPure, Category = "Input|Gameplay")
	FGameplayTagContainer GetBlockedInputTags() const;

	FMCBlockedGameplayInputTagsChangedNativeEvent& OnBlockedInputTagsChanged() { return BlockedInputTagsChangedNativeEvent; }

private:
	void HandleInputPolicyChanged(
		const FGameplayTagContainer& BlockedInputTags,
		const FGameplayTag& ResolvedPolicyTag);
};
