// Fill out your copyright notice in the Description page of Project Settings.

#include "MCGameInstance.h"

#include "Engine/Engine.h"
#include "Engine/World.h"
#include "Service/MCCharacterService.h"
#include "Service/MCUIService.h"

void UMCGameInstance::OnStart()
{
	Super::OnStart();

	if (IsValid(LogService))			LogService->OnStart();
	if (IsValid(DebugService))		DebugService->OnStart();
	if (IsValid(DataTableService))	DataTableService->OnStart();
	if (IsValid(CharacterService))	CharacterService->OnStart();
	if (IsValid(UIService))			UIService->OnStart();
}

UMCGameInstance* UMCGameInstance::GetInstance()
{
	if (!IsValid(GEngine))
	{
		return nullptr;
	}

	for (const FWorldContext& WorldContext : GEngine->GetWorldContexts())
	{
		UWorld* World = WorldContext.World();
		if (!IsValid(World))
		{
			continue;
		}

		UMCGameInstance* GameInstance = Cast<UMCGameInstance>(World->GetGameInstance());
		if (IsValid(GameInstance))
		{
			return GameInstance;
		}
	}

	return nullptr;
}

void UMCGameInstance::Init()
{
	Super::Init();

	LogService			= GetSubsystem<UMCLogService>();
	DebugService		= GetSubsystem<UMCDebugService>();
	DataTableService	= GetSubsystem<UMCDataTableService>();
	CharacterService	= GetSubsystem<UMCCharacterService>();
	UIService			= GetSubsystem<UMCUIService>();

	if (IsValid(LogService))			LogService->Init();
	if (IsValid(DebugService))		DebugService->Init();
	if (IsValid(DataTableService))	DataTableService->Init();
	if (IsValid(CharacterService))	CharacterService->Init();
	if (IsValid(UIService))			UIService->Init();
}
