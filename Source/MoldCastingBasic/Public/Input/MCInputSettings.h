#pragma once

#include "Engine/DeveloperSettings.h"
#include "Input/MCInputTypes.h"
#include "MCInputSettings.generated.h"

UCLASS(Config = Game, DefaultConfig, meta = (DisplayName = "Mold Casting Input"))
class MOLDCASTINGBASIC_API UMCInputSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category = "Policy", meta = (TitleProperty = "PolicyTag"))
	TArray<FMCInputPolicyDefinition> PolicyDefinitions {};

	UMCInputSettings();

	const FMCInputPolicyDefinition* FindPolicyDefinition(const FGameplayTag& PolicyTag) const;

	virtual FName GetCategoryName() const override;
};
