#pragma once

#include "CommonUserWidget.h"
#include "MCUIRootWidget.generated.h"

class SOverlay;
class SWidget;
class UUserWidget;

UCLASS()
class MOLDCASTINGBASIC_API UMCUIRootWidget : public UCommonUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(Transient)
	TArray<TObjectPtr<UUserWidget>> ManagedWidgets {};

	TMap<TWeakObjectPtr<UUserWidget>, int32> WidgetZOrders {};
	TMap<TWeakObjectPtr<UUserWidget>, TSharedPtr<SWidget>> WidgetSlateContents {};
	TSharedPtr<SOverlay> RootOverlay {};

public:
	bool AddWidget(UUserWidget* Widget, int32 ZOrder);

	bool RemoveWidget(UUserWidget* Widget);

	void ClearWidgets();

	UFUNCTION(BlueprintPure, Category = "UI")
	int32 GetWidgetCount() const;

protected:
	virtual TSharedRef<SWidget> RebuildWidget() override;

	virtual void ReleaseSlateResources(bool bReleaseChildren) override;

private:
	void AddWidgetToSlate(UUserWidget* Widget, int32 ZOrder);
};
