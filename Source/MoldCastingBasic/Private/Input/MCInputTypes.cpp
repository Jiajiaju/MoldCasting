#include "Input/MCInputTypes.h"

bool FMCInputPolicyDefinition::BlocksInputTag(const FGameplayTag& InputTag) const
{
	if (!InputTag.IsValid())
	{
		return false;
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
