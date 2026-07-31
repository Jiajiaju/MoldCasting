#pragma once

#include "UI/Widget/Base/MCBasePageWidget.h"
#include "MCTestPageWidget.generated.h"

UCLASS(BlueprintType, Blueprintable)
class MOLDCASTINGBASIC_API UMCTestPageWidget : public UMCBasePageWidget
{
	GENERATED_BODY()

private:
	int32 ConfirmActionCount = 0;
	int32 BackActionCount = 0;

public:
	UMCTestPageWidget(const FObjectInitializer& ObjectInitializer);

	int32 GetConfirmActionCount() const { return ConfirmActionCount; }

	int32 GetBackActionCount() const { return BackActionCount; }

protected:
	virtual void NativeConstruct() override;

private:
	void HandleConfirmAction();

	void HandleBackAction();
};
