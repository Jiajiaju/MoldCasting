#include "DataAsset/Character/MCDataAsset_Input.h"

#include "InputAction.h"

const UInputAction* UMCDataAsset_Input::FindInputActionForTag(
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
