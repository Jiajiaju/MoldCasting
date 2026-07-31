// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MCBaseService.h"
#include "MCDebugService.generated.h"

struct IConsoleCommand;

/**
 *
 */
UCLASS()
class MOLDCASTINGBASIC_API UMCDebugService : public UMCBaseService
{
	GENERATED_BODY()

private:
	TArray<IConsoleCommand*> ScriptConsoleCommands {};

public:
	virtual void OnStart() override;
	virtual void Deinitialize() override;

	void RefreshScriptConsoleCommands();

private:
	void UnregisterScriptConsoleCommands();
};
