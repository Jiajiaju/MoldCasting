#pragma once

#include "CommonActivatableWidget.h"
#include "Input/MCInputTypes.h"
#include "MCBasePageWidget.generated.h"

class UMCInputPolicySubsystem;

UCLASS(Abstract, BlueprintType, Blueprintable)
class MOLDCASTINGBASIC_API UMCBasePageWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Policy", meta = (Categories = "Input.Policy"))
	FGameplayTag InputPolicyTag {};

private:
	FMCInputPolicyHandle InputPolicyHandle {};
	TWeakObjectPtr<UMCInputPolicySubsystem> InputPolicySubsystem {};

public:
	UMCBasePageWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeOnActivated() override;

	virtual void NativeOnDeactivated() override;

private:
	void AcquireInputPolicy();

	void ReleaseInputPolicy();
};
