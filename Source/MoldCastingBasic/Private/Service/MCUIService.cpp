#include "Service/MCUIService.h"

#include "Blueprint/UserWidget.h"
#include "Engine/Engine.h"
#include "Engine/GameInstance.h"
#include "Engine/GameViewportClient.h"
#include "Engine/World.h"
#include "HAL/IConsoleManager.h"
#include "HAL/PlatformMisc.h"
#include "Kismet/GameplayStatics.h"
#include "UI/Widget/Base/MCUIRootWidget.h"

DEFINE_LOG_CATEGORY_STATIC(LogMCUI, Log, All);

namespace
{
	const FName UILevelIndependenceTestMap { TEXT("/Engine/Maps/Entry") };
	const FName UIRootWidgetName { TEXT("MCUIRootWidget") };
	constexpr int32 UIRootWidgetZOrder = 0;
	constexpr uint8 SuccessfulTestExitCode = 0;
	constexpr uint8 FailedTestExitCode = 1;

#if !UE_BUILD_SHIPPING
	UMCUIService* GetUIServiceForConsoleCommand()
	{
		if (!IsValid(GEngine))
		{
			return nullptr;
		}

		const UGameViewportClient* GameViewportClient = GEngine->GameViewport;
		if (!IsValid(GameViewportClient))
		{
			return nullptr;
		}

		const UGameInstance* GameInstance = GameViewportClient->GetGameInstance();
		return IsValid(GameInstance)
			? GameInstance->GetSubsystem<UMCUIService>()
			: nullptr;
	}

	void RunUILevelIndependenceTest()
	{
		UMCUIService* UIService = GetUIServiceForConsoleCommand();
		if (!IsValid(UIService)
			|| !UIService->BeginLevelIndependenceTest(true))
		{
			UE_LOG(LogMCUI, Error, TEXT("UI level independence test could not start."));
			FPlatformMisc::RequestExitWithStatus(false, FailedTestExitCode);
		}
	}

	FAutoConsoleCommand UILevelIndependenceTestCommand(
		TEXT("MC.UI.TestLevelIndependence"),
		TEXT("Validates that the UI root survives travel to the engine entry map."),
		FConsoleCommandDelegate::CreateStatic(&RunUILevelIndependenceTest));
#endif // !UE_BUILD_SHIPPING
}

void UMCUIService::Init()
{
	Super::Init();

	if (!ViewportCreatedDelegateHandle.IsValid())
	{
		ViewportCreatedDelegateHandle =
			UGameViewportClient::OnViewportCreated().AddUObject(
				this,
				&UMCUIService::HandleViewportCreated);
	}

	if (!PostLoadMapDelegateHandle.IsValid())
	{
		PostLoadMapDelegateHandle =
			FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(
				this,
				&UMCUIService::HandlePostLoadMapWithWorld);
	}

	EnsureRootWidget();
	AttachRootWidget();
}

void UMCUIService::OnStart()
{
	Super::OnStart();

	EnsureRootWidget();
	AttachRootWidget();
}

void UMCUIService::Deinitialize()
{
	if (ViewportCreatedDelegateHandle.IsValid())
	{
		UGameViewportClient::OnViewportCreated().Remove(ViewportCreatedDelegateHandle);
		ViewportCreatedDelegateHandle.Reset();
	}

	if (PostLoadMapDelegateHandle.IsValid())
	{
		FCoreUObjectDelegates::PostLoadMapWithWorld.Remove(PostLoadMapDelegateHandle);
		PostLoadMapDelegateHandle.Reset();
	}

	DetachRootWidget();

	if (IsValid(RootWidget))
	{
		RootWidget->ClearWidgets();
	}

	RootWidget = nullptr;
	TestRootWidget.Reset();
	bLevelIndependenceTestPending = false;
	bExitWhenLevelIndependenceTestCompletes = false;

	Super::Deinitialize();
}

UMCUIRootWidget* UMCUIService::GetRootWidget() const
{
	return RootWidget;
}

UUserWidget* UMCUIService::CreateAndAddWidget(
	TSubclassOf<UUserWidget> WidgetClass,
	int32 ZOrder)
{
	UGameInstance* GameInstance = GetGameInstance();
	if (!IsValid(GameInstance) || !WidgetClass)
	{
		return nullptr;
	}

	EnsureRootWidget();
	if (!IsValid(RootWidget))
	{
		return nullptr;
	}

	UUserWidget* Widget = CreateWidget<UUserWidget>(GameInstance, WidgetClass);
	if (!IsValid(Widget) || !RootWidget->AddWidget(Widget, ZOrder))
	{
		return nullptr;
	}

	return Widget;
}

bool UMCUIService::AddWidget(UUserWidget* Widget, int32 ZOrder)
{
	EnsureRootWidget();
	return IsValid(RootWidget)
		&& RootWidget->AddWidget(Widget, ZOrder);
}

bool UMCUIService::RemoveWidget(UUserWidget* Widget)
{
	return IsValid(RootWidget)
		&& RootWidget->RemoveWidget(Widget);
}

void UMCUIService::ClearWidgets()
{
	if (IsValid(RootWidget))
	{
		RootWidget->ClearWidgets();
	}
}

bool UMCUIService::IsRootWidgetAttached() const
{
	const UGameInstance* GameInstance = GetGameInstance();
	const UGameViewportClient* GameViewportClient = IsValid(GameInstance)
		? GameInstance->GetGameViewportClient()
		: nullptr;

	return IsValid(RootWidget)
		&& RootSlateWidget.IsValid()
		&& AttachedViewportClient.IsValid()
		&& AttachedViewportClient.Get() == GameViewportClient;
}

bool UMCUIService::ValidateRootWidget(FString& OutFailureReason) const
{
	OutFailureReason.Reset();

	const UGameInstance* GameInstance = GetGameInstance();
	if (!IsValid(GameInstance))
	{
		OutFailureReason = TEXT("UIService has no valid GameInstance.");
		return false;
	}

	if (!IsValid(RootWidget))
	{
		OutFailureReason = TEXT("RootWidget has not been created.");
		return false;
	}

	if (RootWidget->GetOuter() != GameInstance)
	{
		OutFailureReason = FString::Printf(
			TEXT("RootWidget outer is [%s], expected GameInstance [%s]."),
			*GetNameSafe(RootWidget->GetOuter()),
			*GetNameSafe(GameInstance));
		return false;
	}

	if (!IsRootWidgetAttached())
	{
		OutFailureReason = TEXT("RootWidget is not attached to the GameViewport.");
		return false;
	}

	return true;
}

bool UMCUIService::BeginLevelIndependenceTest(bool bExitOnCompletion)
{
#if UE_BUILD_SHIPPING
	return false;
#else // !UE_BUILD_SHIPPING
	if (bLevelIndependenceTestPending)
	{
		UE_LOG(LogMCUI, Warning, TEXT("UI level independence test is already running."));
		return false;
	}

	EnsureRootWidget();
	AttachRootWidget();

	FString FailureReason;
	if (!ValidateRootWidget(FailureReason))
	{
		UE_LOG(
			LogMCUI,
			Error,
			TEXT("UI level independence pre-travel validation failed: %s"),
			*FailureReason);
		return false;
	}

	if (!IsValid(GetWorld()))
	{
		UE_LOG(
			LogMCUI,
			Error,
			TEXT("UI level independence test requires a World only to initiate travel."));
		return false;
	}

	TestRootWidget = RootWidget;
	bLevelIndependenceTestPending = true;
	bExitWhenLevelIndependenceTestCompletes = bExitOnCompletion;

	UE_LOG(
		LogMCUI,
		Display,
		TEXT("UI level independence test started. RootWidget=[%s], Outer=[%s]."),
		*GetNameSafe(RootWidget),
		*GetNameSafe(RootWidget->GetOuter()));

	UGameplayStatics::OpenLevel(this, UILevelIndependenceTestMap);
	return true;
#endif // UE_BUILD_SHIPPING
}

void UMCUIService::EnsureRootWidget()
{
	if (IsValid(RootWidget))
	{
		return;
	}

	UGameInstance* GameInstance = GetGameInstance();
	if (!IsValid(GameInstance))
	{
		return;
	}

	RootWidget = CreateWidget<UMCUIRootWidget>(
		GameInstance,
		UMCUIRootWidget::StaticClass(),
		UIRootWidgetName);

	if (IsValid(RootWidget))
	{
		UE_LOG(
			LogMCUI,
			Display,
			TEXT("Created RootWidget with GameInstance outer. World=[%s]."),
			*GetNameSafe(GameInstance->GetWorld()));
	}
}

void UMCUIService::AttachRootWidget()
{
	UGameInstance* GameInstance = GetGameInstance();
	UGameViewportClient* GameViewportClient = IsValid(GameInstance)
		? GameInstance->GetGameViewportClient()
		: nullptr;

	if (!IsValid(RootWidget) || !IsValid(GameViewportClient))
	{
		return;
	}

	if (AttachedViewportClient.Get() == GameViewportClient
		&& RootSlateWidget.IsValid())
	{
		return;
	}

	DetachRootWidget();

	RootSlateWidget = RootWidget->TakeWidget();
	GameViewportClient->AddViewportWidgetContent(
		RootSlateWidget.ToSharedRef(),
		UIRootWidgetZOrder);
	AttachedViewportClient = GameViewportClient;

	UE_LOG(
		LogMCUI,
		Display,
		TEXT("Attached RootWidget to GameViewport [%s]."),
		*GetNameSafe(GameViewportClient));
}

void UMCUIService::DetachRootWidget()
{
	if (AttachedViewportClient.IsValid() && RootSlateWidget.IsValid())
	{
		AttachedViewportClient->RemoveViewportWidgetContent(
			RootSlateWidget.ToSharedRef());
	}

	AttachedViewportClient.Reset();
	RootSlateWidget.Reset();
}

void UMCUIService::HandleViewportCreated()
{
	EnsureRootWidget();

	DetachRootWidget();
	AttachRootWidget();
}

void UMCUIService::HandlePostLoadMapWithWorld(UWorld* LoadedWorld)
{
	if (!IsValid(LoadedWorld)
		|| LoadedWorld->GetGameInstance() != GetGameInstance())
	{
		return;
	}

	EnsureRootWidget();
	AttachRootWidget();

	if (bLevelIndependenceTestPending)
	{
		CompleteLevelIndependenceTest();
	}
}

void UMCUIService::CompleteLevelIndependenceTest()
{
	const bool bRootWidgetUnchanged =
		TestRootWidget.IsValid()
		&& TestRootWidget.Get() == RootWidget;

	FString FailureReason;
	const bool bRootWidgetValid = ValidateRootWidget(FailureReason);
	const bool bTestPassed = bRootWidgetUnchanged && bRootWidgetValid;

	if (bTestPassed)
	{
		UE_LOG(
			LogMCUI,
			Display,
			TEXT("UI_LEVEL_INDEPENDENCE_TEST_PASSED: RootWidget survived map travel."));
	}
	else
	{
		UE_LOG(
			LogMCUI,
			Error,
			TEXT("UI_LEVEL_INDEPENDENCE_TEST_FAILED: SameRoot=%s, Validation=%s."),
			bRootWidgetUnchanged ? TEXT("true") : TEXT("false"),
			FailureReason.IsEmpty() ? TEXT("failed") : *FailureReason);
	}

	const bool bShouldExit = bExitWhenLevelIndependenceTestCompletes;
	TestRootWidget.Reset();
	bLevelIndependenceTestPending = false;
	bExitWhenLevelIndependenceTestCompletes = false;

	if (bShouldExit)
	{
		FPlatformMisc::RequestExitWithStatus(
			false,
			bTestPassed ? SuccessfulTestExitCode : FailedTestExitCode);
	}
}
