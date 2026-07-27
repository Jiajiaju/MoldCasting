// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Actor/Base/MCRoleCharacter.h"

#include "Character/Actor/Component/Role/MCRoleMoverComponent.h"

AMCRoleCharacter::AMCRoleCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UMCRoleMoverComponent>(MoverComponentName))
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMCRoleCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AMCRoleCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMCRoleCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
