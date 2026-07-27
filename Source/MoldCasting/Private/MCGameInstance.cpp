// Fill out your copyright notice in the Description page of Project Settings.


#include "MCGameInstance.h"

#include "Engine/Engine.h"
#include "Engine/World.h"

void UMCGameInstance::OnStart()
{
	Super::OnStart();

	if (IsValid(CharacterService)) CharacterService->OnStart();
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

		if (UMCGameInstance* GameInstance = Cast<UMCGameInstance>(World->GetGameInstance()))
		{
			return GameInstance;
		}
	}

	return nullptr;
}

void UMCGameInstance::Init()
{
	Super::Init();

	CharacterService = GetSubsystem<UMCCharacterService>();

	if (IsValid(CharacterService)) CharacterService->Init();
}
