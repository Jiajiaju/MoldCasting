#pragma once

#include "Service/MCBaseService.h"
#include "Templates/SubclassOf.h"
#include "MCUIService.generated.h"

class UGameViewportClient;
class UMCUIRootWidget;
class UUserWidget;
class UWorld;
class SWidget;

UCLASS()
class MOLDCASTINGBASIC_API UMCUIService : public UMCBaseService
{
	GENERATED_BODY()

private:
	UPROPERTY(Transient)
	TObjectPtr<UMCUIRootWidget> RootWidget = nullptr;

	TSharedPtr<SWidget> RootSlateWidget {};
	TWeakObjectPtr<UGameViewportClient> AttachedViewportClient {};
	TWeakObjectPtr<UMCUIRootWidget> TestRootWidget {};
	FDelegateHandle ViewportCreatedDelegateHandle {};
	FDelegateHandle PostLoadMapDelegateHandle {};
	bool bLevelIndependenceTestPending = false;
	bool bExitWhenLevelIndependenceTestCompletes = false;

public:
	virtual void Init() override;

	virtual void OnStart() override;

	virtual void Deinitialize() override;

	UFUNCTION(BlueprintPure, Category = "UI")
	UMCUIRootWidget* GetRootWidget() const;

	UFUNCTION(
		BlueprintCallable,
		Category = "UI",
		meta = (DeterminesOutputType = "WidgetClass"))
	UUserWidget* CreateAndAddWidget(
		TSubclassOf<UUserWidget> WidgetClass,
		int32 ZOrder = 0);

	UFUNCTION(BlueprintCallable, Category = "UI")
	bool AddWidget(UUserWidget* Widget, int32 ZOrder = 0);

	UFUNCTION(BlueprintCallable, Category = "UI")
	bool RemoveWidget(UUserWidget* Widget);

	UFUNCTION(BlueprintCallable, Category = "UI")
	void ClearWidgets();

	UFUNCTION(BlueprintPure, Category = "UI")
	bool IsRootWidgetAttached() const;

	UFUNCTION(BlueprintCallable, Category = "UI|Test", meta = (DevelopmentOnly))
	bool ValidateRootWidget(FString& OutFailureReason) const;

	UFUNCTION(BlueprintCallable, Category = "UI|Test", meta = (DevelopmentOnly))
	bool BeginLevelIndependenceTest(bool bExitOnCompletion = false);

private:
	void EnsureRootWidget();

	void AttachRootWidget();

	void DetachRootWidget();

	void HandleViewportCreated();

	void HandlePostLoadMapWithWorld(UWorld* LoadedWorld);

	void CompleteLevelIndependenceTest();
};
