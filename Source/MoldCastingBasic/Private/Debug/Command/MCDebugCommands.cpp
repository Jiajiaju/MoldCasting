// Fill out your copyright notice in the Description page of Project Settings.

#include "Service/MCDebugService.h"

#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "HAL/IConsoleManager.h"

namespace
{
	void RefreshScriptCommands(UWorld* InWorld)
	{
		if (!IsValid(InWorld)) return;

		UGameInstance* GameInstance = InWorld->GetGameInstance();
		if (!IsValid(GameInstance)) return;

		UMCDebugService* DebugService = GameInstance->GetSubsystem<UMCDebugService>();
		if (IsValid(DebugService))
		{
			DebugService->RefreshScriptConsoleCommands();
		}
	}

	FAutoConsoleCommandWithWorld RefreshScriptCommandsCommand(
		TEXT("mc.Debug.RefreshScriptCommands"),
		TEXT("Re-scan and re-register all AngelScript console commands. Use after an AngelScript hot reload."),
		FConsoleCommandWithWorldDelegate::CreateStatic(&RefreshScriptCommands));
}
