// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Actor/Component/Role/MCRoleInputTypes.h"

namespace MCRoleInput
{
	constexpr int64 BooleanBitCount = 1;
	constexpr float InterpolationSelectionThreshold = 0.5f;
}

FMoverDataStructBase* FMCRoleMoverInputs::Clone() const
{
	return new FMCRoleMoverInputs(*this);
}

bool FMCRoleMoverInputs::NetSerialize(FArchive& Archive, UPackageMap* Map, bool& bOutSuccess)
{
	Super::NetSerialize(Archive, Map, bOutSuccess);

	Archive.SerializeBits(&bWantsToCrouch, MCRoleInput::BooleanBitCount);
	Archive.SerializeBits(&bWantsToSprint, MCRoleInput::BooleanBitCount);
	Archive.SerializeBits(&bWantsToWalk, MCRoleInput::BooleanBitCount);
	Archive.SerializeBits(&bWantsToStrafe, MCRoleInput::BooleanBitCount);
	Archive.SerializeBits(&bWantsToAim, MCRoleInput::BooleanBitCount);
	Archive.SerializeBits(&bTraverseJustPressed, MCRoleInput::BooleanBitCount);
	Archive.SerializeBits(&bInteractJustPressed, MCRoleInput::BooleanBitCount);

	bOutSuccess = true;
	return true;
}

UScriptStruct* FMCRoleMoverInputs::GetScriptStruct() const
{
	return StaticStruct();
}

void FMCRoleMoverInputs::ToString(FAnsiStringBuilderBase& Out) const
{
	Super::ToString(Out);

	Out.Appendf("bWantsToCrouch: %i\n", bWantsToCrouch);
	Out.Appendf("bWantsToSprint: %i\n", bWantsToSprint);
	Out.Appendf("bWantsToWalk: %i\n", bWantsToWalk);
	Out.Appendf("bWantsToStrafe: %i\n", bWantsToStrafe);
	Out.Appendf("bWantsToAim: %i\n", bWantsToAim);
	Out.Appendf("bTraverseJustPressed: %i\n", bTraverseJustPressed);
	Out.Appendf("bInteractJustPressed: %i\n", bInteractJustPressed);
}

bool FMCRoleMoverInputs::ShouldReconcile(const FMoverDataStructBase& AuthorityState) const
{
	const FMCRoleMoverInputs& AuthorityInputs = static_cast<const FMCRoleMoverInputs&>(AuthorityState);

	return bWantsToCrouch != AuthorityInputs.bWantsToCrouch
		|| bWantsToSprint != AuthorityInputs.bWantsToSprint
		|| bWantsToWalk != AuthorityInputs.bWantsToWalk
		|| bWantsToStrafe != AuthorityInputs.bWantsToStrafe
		|| bWantsToAim != AuthorityInputs.bWantsToAim
		|| bTraverseJustPressed != AuthorityInputs.bTraverseJustPressed
		|| bInteractJustPressed != AuthorityInputs.bInteractJustPressed;
}

void FMCRoleMoverInputs::Interpolate(
	const FMoverDataStructBase& From,
	const FMoverDataStructBase& To,
	float Pct)
{
	const FMCRoleMoverInputs& FromInputs = static_cast<const FMCRoleMoverInputs&>(From);
	const FMCRoleMoverInputs& ToInputs = static_cast<const FMCRoleMoverInputs&>(To);
	const FMCRoleMoverInputs& SelectedInputs =
		Pct < MCRoleInput::InterpolationSelectionThreshold ? FromInputs : ToInputs;

	bWantsToCrouch = SelectedInputs.bWantsToCrouch;
	bWantsToSprint = SelectedInputs.bWantsToSprint;
	bWantsToWalk = SelectedInputs.bWantsToWalk;
	bWantsToStrafe = SelectedInputs.bWantsToStrafe;
	bWantsToAim = SelectedInputs.bWantsToAim;
	bTraverseJustPressed = FromInputs.bTraverseJustPressed || ToInputs.bTraverseJustPressed;
	bInteractJustPressed = FromInputs.bInteractJustPressed || ToInputs.bInteractJustPressed;
}

void FMCRoleMoverInputs::Merge(const FMoverDataStructBase& From)
{
	const FMCRoleMoverInputs& FromInputs = static_cast<const FMCRoleMoverInputs&>(From);

	bWantsToCrouch |= FromInputs.bWantsToCrouch;
	bWantsToSprint |= FromInputs.bWantsToSprint;
	bWantsToWalk |= FromInputs.bWantsToWalk;
	bWantsToStrafe |= FromInputs.bWantsToStrafe;
	bWantsToAim |= FromInputs.bWantsToAim;
	bTraverseJustPressed |= FromInputs.bTraverseJustPressed;
	bInteractJustPressed |= FromInputs.bInteractJustPressed;
}
