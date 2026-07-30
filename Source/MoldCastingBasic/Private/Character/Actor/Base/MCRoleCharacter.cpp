// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Actor/Base/MCRoleCharacter.h"

#include "Character/Actor/Component/Role/MCRoleInputComponent.h"
#include "Character/Actor/Component/Role/MCRoleInputConfig.h"
#include "Character/Actor/Component/Role/MCRoleMoverComponent.h"

AMCRoleCharacter::AMCRoleCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UMCRoleMoverComponent>(MoverComponentName))
{
	OverrideInputComponentClass = UMCRoleInputComponent::StaticClass();
}

void AMCRoleCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (!IsLocallyControlled())
	{
		return;
	}

	UMCRoleInputComponent* RoleInputComponent = Cast<UMCRoleInputComponent>(PlayerInputComponent);
	UMCRoleMoverComponent* RoleMoverComponent = GetRoleMoverComponent();

	if (!ensure(IsValid(RoleInputComponent))
		|| !ensure(IsValid(RoleMoverComponent))
		|| !ensure(IsValid(RoleInputConfig.Get())))
	{
		return;
	}

	RoleInputComponent->InitializeRoleInput(this, RoleMoverComponent, RoleInputConfig.Get());
}

void AMCRoleCharacter::UnPossessed()
{
	UMCRoleInputComponent* RoleInputComponent = GetRoleInputComponent();
	if (IsValid(RoleInputComponent))
	{
		RoleInputComponent->ShutdownRoleInput();
	}

	Super::UnPossessed();
}

UMCRoleInputComponent* AMCRoleCharacter::GetRoleInputComponent() const
{
	return Cast<UMCRoleInputComponent>(InputComponent);
}

UMCRoleMoverComponent* AMCRoleCharacter::GetRoleMoverComponent() const
{
	return Cast<UMCRoleMoverComponent>(MoverComponent);
}
