// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MCBaseCharacter.generated.h"

class UCapsuleComponent;
class UMCBaseMoverComponent;
class UMotionWarpingComponent;
class UNavMoverComponent;
class USkeletalMeshComponent;

UCLASS()
class MOLDCASTINGBASIC_API AMCBaseCharacter : public APawn
{
	GENERATED_BODY()

public:
	AMCBaseCharacter(const FObjectInitializer& ObjectInitializer);

protected:
	static const FName MoverComponentName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Components")
	TObjectPtr<UCapsuleComponent> CapsuleComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Components")
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Components")
	TObjectPtr<UMCBaseMoverComponent> MoverComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Components")
	TObjectPtr<UNavMoverComponent> NavMoverComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Components")
	TObjectPtr<UMotionWarpingComponent> MotionWarpingComponent;

	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
};
