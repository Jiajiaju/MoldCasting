// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MoverTypes.h"
#include "MCRoleMoverInputs.generated.h"

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
