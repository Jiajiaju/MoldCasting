#include "UI/Widget/Test/MCTestPageWidget.h"

#include "Common/MCGameplayTags.h"
#include "Containers/Ticker.h"
#include "Engine/Engine.h"
#include "Engine/GameInstance.h"
#include "Engine/GameViewportClient.h"
#include "Engine/LocalPlayer.h"
#include "HAL/IConsoleManager.h"
#include "HAL/PlatformMisc.h"
#include "Input/CommonUIActionRouterBase.h"
#include "Input/CommonUIInputSettings.h"
#include "Input/CommonUIInputTypes.h"
#include "Input/MCInputPolicySubsystem.h"
#include "InputCoreTypes.h"
#include "Service/MCUIService.h"
#include "UITag.h"

DEFINE_LOG_CATEGORY_STATIC(LogMCTestPage, Log, All);

namespace
{
	constexpr int32 UIInputFlowTestPageZOrder = 1000;
	constexpr float UIInputFlowActionRouterReadyTimeoutSeconds = 5.0f;
	constexpr uint8 UIInputFlowSuccessfulExitCode = 0;
	constexpr uint8 UIInputFlowFailedExitCode = 1;

#if !UE_BUILD_SHIPPING
	struct FMCUIInputFlowTestState
	{
		TWeakObjectPtr<UMCTestPageWidget> TestPage {};
		TWeakObjectPtr<UMCUIService> UIService {};
		TWeakObjectPtr<UMCInputPolicySubsystem> InputPolicySubsystem {};
		TWeakObjectPtr<UCommonUIActionRouterBase> ActionRouter {};
		float ElapsedTime = 0.0f;
		bool bTestPassed = true;
		bool bExitOnCompletion = false;
	};

	TSharedPtr<FMCUIInputFlowTestState> ActiveUIInputFlowTest {};

	void CheckUIInputFlowTestCondition(
		const TSharedRef<FMCUIInputFlowTestState>& TestState,
		bool bCondition,
		const TCHAR* FailureReason)
	{
		if (!bCondition)
		{
			TestState->bTestPassed = false;
			UE_LOG(
				LogMCTestPage,
				Error,
				TEXT("UI_INPUT_FLOW_TEST_CHECK_FAILED: %s"),
				FailureReason);
		}
	}

	void CompleteUIInputFlowTest(
		const TSharedRef<FMCUIInputFlowTestState>& TestState)
	{
		if (TestState->bTestPassed)
		{
			UE_LOG(LogMCTestPage, Display, TEXT("UI_INPUT_FLOW_TEST_PASSED"));
		}
		else
		{
			UE_LOG(LogMCTestPage, Error, TEXT("UI_INPUT_FLOW_TEST_FAILED"));
		}

		if (ActiveUIInputFlowTest.Get() == &TestState.Get())
		{
			ActiveUIInputFlowTest.Reset();
		}

		if (TestState->bExitOnCompletion)
		{
			FPlatformMisc::RequestExitWithStatus(
				false,
				TestState->bTestPassed
					? UIInputFlowSuccessfulExitCode
					: UIInputFlowFailedExitCode);
		}
	}

	bool TickUIInputFlowTest(
		float DeltaTime,
		TSharedRef<FMCUIInputFlowTestState> TestState)
	{
		TestState->ElapsedTime += DeltaTime;

		UMCTestPageWidget* TestPage = TestState->TestPage.Get();
		UMCUIService* UIService = TestState->UIService.Get();
		UMCInputPolicySubsystem* InputPolicySubsystem =
			TestState->InputPolicySubsystem.Get();
		UCommonUIActionRouterBase* ActionRouter =
			TestState->ActionRouter.Get();
		if (!IsValid(TestPage)
			|| !IsValid(UIService)
			|| !IsValid(InputPolicySubsystem)
			|| !IsValid(ActionRouter))
		{
			CheckUIInputFlowTestCondition(
				TestState,
				false,
				TEXT("A required test object became invalid."));
			CompleteUIInputFlowTest(TestState);
			return false;
		}

		const ERouteUIInputResult ConfirmResult =
			ActionRouter->ProcessInput(EKeys::SpaceBar, IE_Pressed);
		ActionRouter->ProcessInput(EKeys::SpaceBar, IE_Released);
		if (ConfirmResult != ERouteUIInputResult::Handled)
		{
			if (TestState->ElapsedTime
				< UIInputFlowActionRouterReadyTimeoutSeconds)
			{
				return true;
			}

			CheckUIInputFlowTestCondition(
				TestState,
				false,
				TEXT("Confirm input was not consumed before timeout."));
		}
		else
		{
			CheckUIInputFlowTestCondition(
				TestState,
				TestPage->GetConfirmActionCount() == 1,
				TEXT("Confirm action callback was not executed exactly once."));

			const ERouteUIInputResult BackResult =
				ActionRouter->ProcessInput(EKeys::Escape, IE_Pressed);
			ActionRouter->ProcessInput(EKeys::Escape, IE_Released);
			CheckUIInputFlowTestCondition(
				TestState,
				BackResult == ERouteUIInputResult::Handled,
				TEXT("Back input was not consumed by CommonUI."));
			CheckUIInputFlowTestCondition(
				TestState,
				TestPage->GetBackActionCount() == 1,
				TEXT("Back action callback was not executed exactly once."));
		}

		const bool bRemovedFromRoot = UIService->RemoveWidget(TestPage);
		CheckUIInputFlowTestCondition(
			TestState,
			bRemovedFromRoot,
			TEXT("Test page could not be removed from the UI root."));
		CheckUIInputFlowTestCondition(
			TestState,
			!TestPage->IsActivated(),
			TEXT("Test page remained active after removal."));
		CheckUIInputFlowTestCondition(
			TestState,
			InputPolicySubsystem->GetResolvedPolicyTag()
				== MCGameplayTags::Input_Policy_Gameplay,
			TEXT("Gameplay input policy was not restored."));
		CheckUIInputFlowTestCondition(
			TestState,
			!InputPolicySubsystem->IsInputBlocked(
				MCGameplayTags::Input_Gameplay),
			TEXT("Gameplay input remained blocked after page removal."));

		CompleteUIInputFlowTest(TestState);
		return false;
	}

	void RunUIInputFlowTest(const TArray<FString>& Arguments)
	{
		const TSharedRef<FMCUIInputFlowTestState> TestState =
			MakeShared<FMCUIInputFlowTestState>();
		TestState->bExitOnCompletion =
			Arguments.Contains(TEXT("ExitOnCompletion"));

		if (ActiveUIInputFlowTest.IsValid())
		{
			CheckUIInputFlowTestCondition(
				TestState,
				false,
				TEXT("Another UI input flow test is already running."));
			CompleteUIInputFlowTest(TestState);
			return;
		}

		ActiveUIInputFlowTest = TestState;

		if (!IsValid(GEngine) || !IsValid(GEngine->GameViewport))
		{
			CheckUIInputFlowTestCondition(
				TestState,
				false,
				TEXT("GameViewport is invalid."));
			CompleteUIInputFlowTest(TestState);
			return;
		}

		UGameInstance* GameInstance =
			GEngine->GameViewport->GetGameInstance();
		ULocalPlayer* LocalPlayer = IsValid(GameInstance)
			? GameInstance->GetFirstGamePlayer()
			: nullptr;
		UMCUIService* UIService = IsValid(GameInstance)
			? GameInstance->GetSubsystem<UMCUIService>()
			: nullptr;
		UMCInputPolicySubsystem* InputPolicySubsystem =
			IsValid(LocalPlayer)
				? LocalPlayer->GetSubsystem<UMCInputPolicySubsystem>()
				: nullptr;
		UCommonUIActionRouterBase* ActionRouter =
			IsValid(LocalPlayer)
				? LocalPlayer->GetSubsystem<UCommonUIActionRouterBase>()
				: nullptr;

		if (!IsValid(GameInstance)
			|| !IsValid(LocalPlayer)
			|| !IsValid(UIService)
			|| !IsValid(InputPolicySubsystem)
			|| !IsValid(ActionRouter))
		{
			CheckUIInputFlowTestCondition(
				TestState,
				false,
				TEXT("A required input or UI service is invalid."));
			CompleteUIInputFlowTest(TestState);
			return;
		}

		TestState->UIService = UIService;
		TestState->InputPolicySubsystem = InputPolicySubsystem;
		TestState->ActionRouter = ActionRouter;

		const FUIActionTag ConfirmActionTag =
			FUIActionTag::ConvertChecked(
				MCGameplayTags::UI_Action_Confirm);
		const FUIActionTag BackActionTag =
			FUIActionTag::ConvertChecked(
				MCGameplayTags::UI_Action_Back);
		CheckUIInputFlowTestCondition(
			TestState,
			UCommonUIInputSettings::Get().FindAction(
				ConfirmActionTag) != nullptr,
			TEXT("UI.Action.Confirm is not configured."));
		CheckUIInputFlowTestCondition(
			TestState,
			UCommonUIInputSettings::Get().FindAction(
				BackActionTag) != nullptr,
			TEXT("UI.Action.Back is not configured."));

		UMCTestPageWidget* TestPage =
			CreateWidget<UMCTestPageWidget>(GameInstance);
		CheckUIInputFlowTestCondition(
			TestState,
			IsValid(TestPage),
			TEXT("Test page could not be created."));
		if (!IsValid(TestPage))
		{
			CompleteUIInputFlowTest(TestState);
			return;
		}

		TestState->TestPage = TestPage;

		const bool bAddedToRoot =
			UIService->AddWidget(
				TestPage,
				UIInputFlowTestPageZOrder);
		CheckUIInputFlowTestCondition(
			TestState,
			bAddedToRoot,
			TEXT("Test page could not be added to the UI root."));
		if (!bAddedToRoot)
		{
			CompleteUIInputFlowTest(TestState);
			return;
		}

		CheckUIInputFlowTestCondition(
			TestState,
			TestPage->IsActivated(),
			TEXT("Test page was not activated after being added."));
		CheckUIInputFlowTestCondition(
			TestState,
			InputPolicySubsystem->GetResolvedPolicyTag()
				== MCGameplayTags::Input_Policy_UIOnly_PauseMenu,
			TEXT("Pause menu input policy was not resolved."));
		CheckUIInputFlowTestCondition(
			TestState,
			InputPolicySubsystem->IsInputBlocked(
				MCGameplayTags::Input_Gameplay),
			TEXT("Gameplay input was not blocked."));

		FTSTicker::GetCoreTicker().AddTicker(
			FTickerDelegate::CreateStatic(
				&TickUIInputFlowTest,
				TestState));
	}

	FAutoConsoleCommand UIInputFlowTestCommand(
		TEXT("MC.UI.TestInputFlow"),
		TEXT("Validates CommonUI action consumption and gameplay input policy lifecycle. Pass ExitOnCompletion for unattended runs."),
		FConsoleCommandWithArgsDelegate::CreateStatic(
			&RunUIInputFlowTest));
#endif // !UE_BUILD_SHIPPING
}

UMCTestPageWidget::UMCTestPageWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	InputPolicyTag = MCGameplayTags::Input_Policy_UIOnly_PauseMenu;
}

void UMCTestPageWidget::NativeConstruct()
{
	Super::NativeConstruct();

	FBindUIActionArgs ConfirmBindArgs(
		FUIActionTag::ConvertChecked(MCGameplayTags::UI_Action_Confirm),
		false,
		FSimpleDelegate::CreateUObject(
			this,
			&UMCTestPageWidget::HandleConfirmAction));
	ConfirmBindArgs.InputMode = ECommonInputMode::Menu;
	ConfirmBindArgs.bConsumeInput = true;
	RegisterUIActionBinding(ConfirmBindArgs);

	FBindUIActionArgs BackBindArgs(
		FUIActionTag::ConvertChecked(MCGameplayTags::UI_Action_Back),
		false,
		FSimpleDelegate::CreateUObject(
			this,
			&UMCTestPageWidget::HandleBackAction));
	BackBindArgs.InputMode = ECommonInputMode::Menu;
	BackBindArgs.bConsumeInput = true;
	RegisterUIActionBinding(BackBindArgs);
}

void UMCTestPageWidget::HandleConfirmAction()
{
	++ConfirmActionCount;
}

void UMCTestPageWidget::HandleBackAction()
{
	++BackActionCount;
}
