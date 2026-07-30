#include "Input/MCGameplayInputRouter.h"

#include "Input/MCInputPolicySubsystem.h"
#include "Subsystems/SubsystemCollection.h"

void UMCGameplayInputRouter::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	InputPolicySubsystem = Collection.InitializeDependency<UMCInputPolicySubsystem>();
	if (ensure(IsValid(InputPolicySubsystem)))
	{
		InputPolicySubsystem->OnInputPolicyChanged().AddUObject(
			this,
			&UMCGameplayInputRouter::HandleInputPolicyChanged);
	}
}

void UMCGameplayInputRouter::Deinitialize()
{
	if (IsValid(InputPolicySubsystem))
	{
		InputPolicySubsystem->OnInputPolicyChanged().RemoveAll(this);
	}

	InputPolicySubsystem = nullptr;
	BlockedInputTagsChangedNativeEvent.Clear();
	OnBlockedInputTagsChangedEvent.Clear();

	Super::Deinitialize();
}

bool UMCGameplayInputRouter::CanRouteInput(FGameplayTag InputTag) const
{
	return InputTag.IsValid()
		&& (!IsValid(InputPolicySubsystem)
			|| !InputPolicySubsystem->IsInputBlocked(InputTag));
}

bool UMCGameplayInputRouter::RouteInput(FGameplayTag InputTag) const
{
	return CanRouteInput(InputTag);
}

FGameplayTagContainer UMCGameplayInputRouter::GetBlockedInputTags() const
{
	return IsValid(InputPolicySubsystem)
		? InputPolicySubsystem->GetBlockedInputTags()
		: FGameplayTagContainer();
}

void UMCGameplayInputRouter::HandleInputPolicyChanged(
	const FGameplayTagContainer& BlockedInputTags,
	const FGameplayTag& ResolvedPolicyTag)
{
	BlockedInputTagsChangedNativeEvent.Broadcast(BlockedInputTags);
	OnBlockedInputTagsChangedEvent.Broadcast(BlockedInputTags);
}
