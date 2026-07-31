// Fill out your copyright notice in the Description page of Project Settings.


#include "Service/MCDebugService.h"

#include "Debug/Command/MCScriptDebugCommand.h"
#include "HAL/IConsoleManager.h"
#include "UObject/UObjectHash.h"

DEFINE_LOG_CATEGORY_STATIC(LogMCDebugService, Log, All);

void UMCDebugService::OnStart()
{
	Super::OnStart();

	RefreshScriptConsoleCommands();
}

void UMCDebugService::Deinitialize()
{
	UnregisterScriptConsoleCommands();

	Super::Deinitialize();
}

void UMCDebugService::RefreshScriptConsoleCommands()
{
	UnregisterScriptConsoleCommands();

	TArray<UClass*> CommandClasses {};
	GetDerivedClasses(UMCScriptDebugCommand::StaticClass(), CommandClasses, true);

	for (UClass* CommandClass : CommandClasses)
	{
		if (!IsValid(CommandClass)) continue;
		if (CommandClass->HasAnyClassFlags(CLASS_Abstract | CLASS_Deprecated | CLASS_NewerVersionExists)) continue;

		UMCScriptDebugCommand* CommandDefault =
			CommandClass->GetDefaultObject<UMCScriptDebugCommand>();
		if (!IsValid(CommandDefault)) continue;
		if (CommandDefault->CommandName.IsEmpty()) continue;

		IConsoleManager& ConsoleManager = IConsoleManager::Get();
		if (ConsoleManager.FindConsoleObject(*CommandDefault->CommandName))
		{
			UE_LOG(
				LogMCDebugService,
				Warning,
				TEXT("Skipped duplicate script console command '%s'."),
				*CommandDefault->CommandName);
			continue;
		}

		TWeakObjectPtr<UClass> WeakCommandClass = CommandClass;
		IConsoleCommand* RegisteredCommand =
			ConsoleManager.RegisterConsoleCommand(
				*CommandDefault->CommandName,
				*CommandDefault->Help,
				FConsoleCommandWithWorldAndArgsDelegate::CreateLambda(
					[WeakCommandClass](const TArray<FString>& Arguments, UWorld* InWorld)
					{
						if (!IsValid(InWorld)) return;

						UClass* CurrentCommandClass = WeakCommandClass.Get();
						if (!IsValid(CurrentCommandClass)) return;

						UMCScriptDebugCommand* CurrentCommandDefault =
							CurrentCommandClass->GetDefaultObject<UMCScriptDebugCommand>();
						if (IsValid(CurrentCommandDefault))
						{
							CurrentCommandDefault->Execute(Arguments, InWorld);
						}
					}),
				ECVF_Default);
		if (RegisteredCommand)
		{
			ScriptConsoleCommands.Add(RegisteredCommand);
		}
	}
}

void UMCDebugService::UnregisterScriptConsoleCommands()
{
	for (IConsoleCommand* ScriptConsoleCommand : ScriptConsoleCommands)
	{
		if (!ScriptConsoleCommand) continue;

		IConsoleManager::Get().UnregisterConsoleObject(ScriptConsoleCommand);
	}

	ScriptConsoleCommands.Empty();
}
