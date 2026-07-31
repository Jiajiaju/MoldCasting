#include "UI/Widget/Base/MCBasePageWidget.h"

#include "Engine/LocalPlayer.h"
#include "Input/MCInputPolicySubsystem.h"

UMCBasePageWidget::UMCBasePageWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InputMappingPriority = MCInputMappingPriority::UI;
}

void UMCBasePageWidget::NativeOnActivated()
{
	AcquireInputPolicy();
	Super::NativeOnActivated();
}

void UMCBasePageWidget::NativeOnDeactivated()
{
	Super::NativeOnDeactivated();
	ReleaseInputPolicy();
}

void UMCBasePageWidget::AcquireInputPolicy()
{
	ReleaseInputPolicy();

	if (!InputPolicyTag.IsValid())
	{
		return;
	}

	ULocalPlayer* LocalPlayer = GetOwningLocalPlayer();
	if (!IsValid(LocalPlayer))
	{
		return;
	}

	UMCInputPolicySubsystem* Subsystem =
		LocalPlayer->GetSubsystem<UMCInputPolicySubsystem>();
	if (!IsValid(Subsystem))
	{
		return;
	}

	InputPolicySubsystem = Subsystem;
	InputPolicyHandle = Subsystem->AcquirePolicy(InputPolicyTag, this);
}

void UMCBasePageWidget::ReleaseInputPolicy()
{
	if (InputPolicyHandle.IsValid() && InputPolicySubsystem.IsValid())
	{
		InputPolicySubsystem->ReleasePolicy(InputPolicyHandle);
	}

	InputPolicyHandle.Reset();
	InputPolicySubsystem.Reset();
}
