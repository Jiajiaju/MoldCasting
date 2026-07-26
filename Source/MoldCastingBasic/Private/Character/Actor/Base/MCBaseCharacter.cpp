// Fill out your copyright notice in the Description page of Project Settings.
#include "Character/Actor/Base/MCBaseCharacter.h"

// Sets default values
AMCBaseCharacter::AMCBaseCharacter()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMCBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMCBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AMCBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
