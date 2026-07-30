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

protected:
	static const FName MoverComponentName;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Components")
	TObjectPtr<UCapsuleComponent> CapsuleComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Components")
	TObjectPtr<USkeletalMeshComponent> SkeletalMeshComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Components")
	TObjectPtr<UMCBaseMoverComponent> MoverComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Components")
	TObjectPtr<UNavMoverComponent> NavMoverComponent = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character|Components")
	TObjectPtr<UMotionWarpingComponent> MotionWarpingComponent = nullptr;

public:
	AMCBaseCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
};
