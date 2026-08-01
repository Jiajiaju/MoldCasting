// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MCRoleInputState.generated.h"

USTRUCT(BlueprintType)
struct MOLDCASTINGBASIC_API FMCRoleInputState
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Role|Input")
	FVector2D MoveInput {};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Role|Input")
	FVector2D LookInput {};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Role|Input")
	bool bMoveInputIsWorldSpace = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Role|Input")
	bool bJumpPressed = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Role|Input")
	bool bJumpJustPressed = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Role|Input")
	bool bCrouchPressed = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Role|Input")
	bool bSprintPressed = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Role|Input")
	bool bWalkPressed = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Role|Input")
	bool bStrafePressed = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Role|Input")
	bool bAimPressed = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Role|Input")
	bool bTraverseJustPressed = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Role|Input")
	bool bInteractJustPressed = false;
};
