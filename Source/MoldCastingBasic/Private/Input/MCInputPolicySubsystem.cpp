#include "Input/MCInputPolicySubsystem.h"

#include "Common/MCGameplayTags.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/PlayerController.h"
#include "Input/CommonUIActionRouterBase.h"
#include "Input/MCInputSettings.h"
#include "Subsystems/SubsystemCollection.h"

void UMCInputPolicySubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	CommonUIActionRouter = Collection.InitializeDependency<UCommonUIActionRouterBase>();
	RebuildResolvedPolicy();
}

void UMCInputPolicySubsystem::Deinitialize()
{
	ActivePolicies.Reset();
	BlockedInputTags.Reset();
	ResolvedPolicyDefinition.Reset();
	InputPolicyChangedNativeEvent.Clear();
	OnInputPolicyChangedEvent.Clear();
	CommonUIActionRouter = nullptr;

	Super::Deinitialize();
}

void UMCInputPolicySubsystem::PlayerControllerChanged(APlayerController* NewPlayerController)
{
	Super::PlayerControllerChanged(NewPlayerController);

	if (IsValid(NewPlayerController))
	{
		ApplyResolvedUIInputConfig();
	}
}

FMCInputPolicyHandle UMCInputPolicySubsystem::AcquirePolicy(
	FGameplayTag PolicyTag,
	UObject* Source)
{
	const UMCInputSettings* InputSettings = GetDefault<UMCInputSettings>();
	const FMCInputPolicyDefinition* PolicyDefinition = IsValid(InputSettings)
		? InputSettings->FindPolicyDefinition(PolicyTag)
		: nullptr;

	if (!ensureMsgf(
		PolicyDefinition,
		TEXT("Input policy [%s] is not configured in UMCInputSettings."),
		*PolicyTag.ToString()))
	{
		return FMCInputPolicyHandle();
	}

	FMCInputPolicyHandle Handle;
	Handle.Id = NextPolicyHandleId++;

	FMCActiveInputPolicy ActivePolicy;
	ActivePolicy.Handle = Handle;
	ActivePolicy.Definition = *PolicyDefinition;
	ActivePolicy.Source = Source;
	ActivePolicy.ActivationSequence = NextActivationSequence++;
	ActivePolicy.bHasSource = IsValid(Source);
	ActivePolicies.Add(Handle.Id, MoveTemp(ActivePolicy));

	RebuildResolvedPolicy();

	if (PolicyDefinition->bFlushPressedKeysOnActivation)
	{
		FlushPressedKeys();
	}

	return Handle;
}

bool UMCInputPolicySubsystem::ReleasePolicy(FMCInputPolicyHandle Handle)
{
	if (!Handle.IsValid())
	{
		return false;
	}

	const int32 RemovedPolicyCount = ActivePolicies.Remove(Handle.Id);
	if (RemovedPolicyCount == 0)
	{
		return false;
	}

	RebuildResolvedPolicy();
	FlushPressedKeys();
	return true;
}

void UMCInputPolicySubsystem::ReleasePoliciesForSource(const UObject* Source)
{
	if (!IsValid(Source))
	{
		return;
	}

	bool bRemovedPolicy = false;
	for (auto Iterator = ActivePolicies.CreateIterator(); Iterator; ++Iterator)
	{
		if (Iterator.Value().Source.Get() == Source)
		{
			Iterator.RemoveCurrent();
			bRemovedPolicy = true;
		}
	}

	if (bRemovedPolicy)
	{
		RebuildResolvedPolicy();
		FlushPressedKeys();
	}
}

bool UMCInputPolicySubsystem::IsInputBlocked(FGameplayTag InputTag) const
{
	if (!InputTag.IsValid())
	{
		return true;
	}

	for (const FGameplayTag& BlockedInputTag : BlockedInputTags)
	{
		if (InputTag.MatchesTag(BlockedInputTag))
		{
			return true;
		}
	}

	return false;
}

FGameplayTagContainer UMCInputPolicySubsystem::GetBlockedInputTags() const
{
	return BlockedInputTags;
}

FGameplayTag UMCInputPolicySubsystem::GetResolvedPolicyTag() const
{
	return ResolvedPolicyDefinition.IsSet()
		? ResolvedPolicyDefinition->PolicyTag
		: FGameplayTag();
}

bool UMCInputPolicySubsystem::GetResolvedPolicyDefinition(
	FMCInputPolicyDefinition& OutDefinition) const
{
	if (!ResolvedPolicyDefinition.IsSet())
	{
		return false;
	}

	OutDefinition = ResolvedPolicyDefinition.GetValue();
	return true;
}

bool UMCInputPolicySubsystem::GetPolicyDefinition(
	FGameplayTag PolicyTag,
	FMCInputPolicyDefinition& OutDefinition) const
{
	const UMCInputSettings* InputSettings = GetDefault<UMCInputSettings>();
	const FMCInputPolicyDefinition* PolicyDefinition = IsValid(InputSettings)
		? InputSettings->FindPolicyDefinition(PolicyTag)
		: nullptr;

	if (!PolicyDefinition)
	{
		return false;
	}

	OutDefinition = *PolicyDefinition;
	return true;
}

void UMCInputPolicySubsystem::RebuildResolvedPolicy()
{
	const FGameplayTagContainer PreviousBlockedInputTags = BlockedInputTags;
	const FGameplayTag PreviousResolvedPolicyTag = GetResolvedPolicyTag();

	BlockedInputTags.Reset();
	ResolvedPolicyDefinition.Reset();

	TOptional<FMCInputPolicyDefinition> HighestPriorityPolicyDefinition;
	uint64 HighestPriorityActivationSequence = 0;
	for (auto Iterator = ActivePolicies.CreateIterator(); Iterator; ++Iterator)
	{
		const FMCActiveInputPolicy& ActivePolicy = Iterator.Value();
		if (ActivePolicy.bHasSource && !ActivePolicy.Source.IsValid())
		{
			Iterator.RemoveCurrent();
			continue;
		}

		BlockedInputTags.AppendTags(ActivePolicy.Definition.BlockedInputTags);

		if (!HighestPriorityPolicyDefinition.IsSet()
			|| ActivePolicy.Definition.Priority > HighestPriorityPolicyDefinition->Priority
			|| (ActivePolicy.Definition.Priority == HighestPriorityPolicyDefinition->Priority
				&& ActivePolicy.ActivationSequence > HighestPriorityActivationSequence))
		{
			HighestPriorityPolicyDefinition = ActivePolicy.Definition;
			HighestPriorityActivationSequence = ActivePolicy.ActivationSequence;
		}
	}

	if (HighestPriorityPolicyDefinition.IsSet())
	{
		ResolvedPolicyDefinition = HighestPriorityPolicyDefinition.GetValue();
	}
	else
	{
		const UMCInputSettings* InputSettings = GetDefault<UMCInputSettings>();
		const FMCInputPolicyDefinition* GameplayPolicyDefinition = IsValid(InputSettings)
			? InputSettings->FindPolicyDefinition(MCGameplayTags::Input_Policy_Gameplay)
			: nullptr;

		if (GameplayPolicyDefinition)
		{
			ResolvedPolicyDefinition = *GameplayPolicyDefinition;
		}
	}

	const FGameplayTag ResolvedPolicyTag = GetResolvedPolicyTag();
	if (PreviousBlockedInputTags != BlockedInputTags
		|| PreviousResolvedPolicyTag != ResolvedPolicyTag)
	{
		ApplyResolvedUIInputConfig();
		InputPolicyChangedNativeEvent.Broadcast(BlockedInputTags, ResolvedPolicyTag);
		OnInputPolicyChangedEvent.Broadcast(BlockedInputTags, ResolvedPolicyTag);
	}
}

void UMCInputPolicySubsystem::ApplyResolvedUIInputConfig()
{
	if (!IsValid(CommonUIActionRouter) || !ResolvedPolicyDefinition.IsSet())
	{
		return;
	}

	const ULocalPlayer* LocalPlayer = GetLocalPlayer();
	if (!IsValid(LocalPlayer)
		|| !IsValid(LocalPlayer->GetPlayerController(GetWorld())))
	{
		return;
	}

	const FMCInputPolicyDefinition& PolicyDefinition = ResolvedPolicyDefinition.GetValue();
	CommonUIActionRouter->SetActiveUIInputConfig(
		FUIInputConfig(
			PolicyDefinition.InputMode,
			PolicyDefinition.MouseCaptureMode,
			PolicyDefinition.MouseLockMode,
			PolicyDefinition.bHideCursorDuringViewportCapture),
		this);
}

void UMCInputPolicySubsystem::FlushPressedKeys() const
{
	const ULocalPlayer* LocalPlayer = GetLocalPlayer();
	APlayerController* PlayerController = IsValid(LocalPlayer)
		? LocalPlayer->GetPlayerController(GetWorld())
		: nullptr;

	if (IsValid(PlayerController))
	{
		PlayerController->FlushPressedKeys();
	}
}
