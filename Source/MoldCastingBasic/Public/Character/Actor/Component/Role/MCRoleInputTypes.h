// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MoverTypes.h"
#include "MCRoleInputTypes.generated.h"

USTRUCT(BlueprintType)
struct MOLDCASTINGBASIC_API FMCRoleInputState
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Role|Input")
	FVector2D MoveInput = FVector2D::ZeroVector;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Role|Input")
	FVector2D LookInput = FVector2D::ZeroVector;

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

USTRUCT(BlueprintType)
struct MOLDCASTINGBASIC_API FMCRoleMoverInputs : public FMoverDataStructBase
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Role|Mover")
	bool bWantsToCrouch = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Role|Mover")
	bool bWantsToSprint = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Role|Mover")
	bool bWantsToWalk = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Role|Mover")
	bool bWantsToStrafe = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Role|Mover")
	bool bWantsToAim = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Role|Mover")
	bool bTraverseJustPressed = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Role|Mover")
	bool bInteractJustPressed = false;

	virtual FMoverDataStructBase* Clone() const override;

	virtual bool NetSerialize(FArchive& Archive, UPackageMap* Map, bool& bOutSuccess) override;

	virtual UScriptStruct* GetScriptStruct() const override;

	virtual void ToString(FAnsiStringBuilderBase& Out) const override;

	virtual bool ShouldReconcile(const FMoverDataStructBase& AuthorityState) const override;

	virtual void Interpolate(const FMoverDataStructBase& From, const FMoverDataStructBase& To, float Pct) override;

	virtual void Merge(const FMoverDataStructBase& From) override;
};

template<>
struct TStructOpsTypeTraits<FMCRoleMoverInputs> : public TStructOpsTypeTraitsBase2<FMCRoleMoverInputs>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true
	};
};
