// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Actor/Base/MCBaseCharacter.h"

#include "Character/Actor/Component/Base/MCBaseMoverComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "DefaultMovementSet/NavMoverComponent.h"
#include "MotionWarpingComponent.h"

const FName AMCBaseCharacter::MoverComponentName(TEXT("MoverComponent"));

AMCBaseCharacter::AMCBaseCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	SetReplicatingMovement(false);

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	SetRootComponent(CapsuleComponent);

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComponent"));
	SkeletalMeshComponent->SetupAttachment(CapsuleComponent);

	MoverComponent = CreateDefaultSubobject<UMCBaseMoverComponent>(MoverComponentName);
	MoverComponent->SetUpdatedComponent(CapsuleComponent);
	MoverComponent->SetPrimaryVisualComponent(SkeletalMeshComponent);

	NavMoverComponent = CreateDefaultSubobject<UNavMoverComponent>(TEXT("NavMoverComponent"));
	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComponent"));
}

void AMCBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AMCBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMCBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
