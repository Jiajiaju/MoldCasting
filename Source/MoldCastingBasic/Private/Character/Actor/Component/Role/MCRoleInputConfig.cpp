#include "Character/Actor/Component/Role/MCRoleInputConfig.h"

#include "InputAction.h"

const UInputAction* UMCRoleInputConfig::FindInputActionForTag(
	const FGameplayTag& InputTag,
	const UInputAction* LegacyInputAction) const
{
	const FMCInputActionDefinition* InputActionDefinition = InputActions.FindByPredicate(
		[InputTag](const FMCInputActionDefinition& Definition)
		{
			return Definition.InputTag == InputTag && IsValid(Definition.InputAction);
		});

	return InputActionDefinition
		? InputActionDefinition->InputAction.Get()
		: LegacyInputAction;
}
