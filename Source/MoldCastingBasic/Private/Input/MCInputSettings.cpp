#include "Input/MCInputSettings.h"

#include "Common/MCGameplayTags.h"

namespace MCInputPolicyPriority
{
	constexpr int32 Gameplay = 0;
	constexpr int32 Inventory = 100;
	constexpr int32 Dialogue = 200;
	constexpr int32 PauseMenu = 1000;
}

UMCInputSettings::UMCInputSettings()
{
	FMCInputPolicyDefinition GameplayPolicy;
	GameplayPolicy.PolicyTag = MCGameplayTags::Input_Policy_Gameplay;
	GameplayPolicy.Priority = MCInputPolicyPriority::Gameplay;
	GameplayPolicy.InputMode = ECommonInputMode::Game;
	GameplayPolicy.MouseCaptureMode = EMouseCaptureMode::CapturePermanently_IncludingInitialMouseDown;
	GameplayPolicy.MouseLockMode = EMouseLockMode::LockOnCapture;
	GameplayPolicy.bFlushPressedKeysOnActivation = false;
	PolicyDefinitions.Add(GameplayPolicy);

	FMCInputPolicyDefinition InventoryPolicy;
	InventoryPolicy.PolicyTag = MCGameplayTags::Input_Policy_Mixed_Inventory;
	InventoryPolicy.Priority = MCInputPolicyPriority::Inventory;
	InventoryPolicy.InputMode = ECommonInputMode::All;
	InventoryPolicy.MouseCaptureMode = EMouseCaptureMode::NoCapture;
	InventoryPolicy.MouseLockMode = EMouseLockMode::DoNotLock;
	InventoryPolicy.BlockedInputTags.AddTag(MCGameplayTags::Input_Gameplay_Move);
	InventoryPolicy.BlockedInputTags.AddTag(MCGameplayTags::Input_Gameplay_Combat);
	InventoryPolicy.BlockedInputTags.AddTag(MCGameplayTags::Input_Gameplay_Interact);
	PolicyDefinitions.Add(InventoryPolicy);

	FMCInputPolicyDefinition DialoguePolicy;
	DialoguePolicy.PolicyTag = MCGameplayTags::Input_Policy_Mixed_Dialogue;
	DialoguePolicy.Priority = MCInputPolicyPriority::Dialogue;
	DialoguePolicy.InputMode = ECommonInputMode::All;
	DialoguePolicy.MouseCaptureMode = EMouseCaptureMode::NoCapture;
	DialoguePolicy.MouseLockMode = EMouseLockMode::DoNotLock;
	DialoguePolicy.BlockedInputTags.AddTag(MCGameplayTags::Input_Gameplay_Combat);
	DialoguePolicy.BlockedInputTags.AddTag(MCGameplayTags::Input_Gameplay_Interact);
	PolicyDefinitions.Add(DialoguePolicy);

	FMCInputPolicyDefinition PauseMenuPolicy;
	PauseMenuPolicy.PolicyTag = MCGameplayTags::Input_Policy_UIOnly_PauseMenu;
	PauseMenuPolicy.Priority = MCInputPolicyPriority::PauseMenu;
	PauseMenuPolicy.InputMode = ECommonInputMode::Menu;
	PauseMenuPolicy.MouseCaptureMode = EMouseCaptureMode::NoCapture;
	PauseMenuPolicy.MouseLockMode = EMouseLockMode::DoNotLock;
	PauseMenuPolicy.BlockedInputTags.AddTag(MCGameplayTags::Input_Gameplay);
	PolicyDefinitions.Add(PauseMenuPolicy);
}

const FMCInputPolicyDefinition* UMCInputSettings::FindPolicyDefinition(
	const FGameplayTag& PolicyTag) const
{
	return PolicyDefinitions.FindByPredicate(
		[PolicyTag](const FMCInputPolicyDefinition& Definition)
		{
			return Definition.PolicyTag == PolicyTag;
		});
}

FName UMCInputSettings::GetCategoryName() const
{
	return TEXT("Game");
}
