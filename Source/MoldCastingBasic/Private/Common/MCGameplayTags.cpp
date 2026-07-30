// Fill out your copyright notice in the Description page of Project Settings.


#include "Common/MCGameplayTags.h"

namespace MCGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG(MC_Test, "XY.Test");

	UE_DEFINE_GAMEPLAY_TAG(Input_Policy_Gameplay, "Input.Policy.Gameplay");
	UE_DEFINE_GAMEPLAY_TAG(Input_Policy_Mixed_Inventory, "Input.Policy.Mixed.Inventory");
	UE_DEFINE_GAMEPLAY_TAG(Input_Policy_Mixed_Dialogue, "Input.Policy.Mixed.Dialogue");
	UE_DEFINE_GAMEPLAY_TAG(Input_Policy_UIOnly_PauseMenu, "Input.Policy.UIOnly.PauseMenu");

	UE_DEFINE_GAMEPLAY_TAG(Input_Gameplay, "Input.Gameplay");
	UE_DEFINE_GAMEPLAY_TAG(Input_Gameplay_Move, "Input.Gameplay.Move");
	UE_DEFINE_GAMEPLAY_TAG(Input_Gameplay_Move_WorldSpace, "Input.Gameplay.Move.WorldSpace");
	UE_DEFINE_GAMEPLAY_TAG(Input_Gameplay_Look, "Input.Gameplay.Look");
	UE_DEFINE_GAMEPLAY_TAG(Input_Gameplay_Look_Mouse, "Input.Gameplay.Look.Mouse");
	UE_DEFINE_GAMEPLAY_TAG(Input_Gameplay_Look_Gamepad, "Input.Gameplay.Look.Gamepad");
	UE_DEFINE_GAMEPLAY_TAG(Input_Gameplay_Jump, "Input.Gameplay.Jump");
	UE_DEFINE_GAMEPLAY_TAG(Input_Gameplay_Crouch, "Input.Gameplay.Crouch");
	UE_DEFINE_GAMEPLAY_TAG(Input_Gameplay_Sprint, "Input.Gameplay.Sprint");
	UE_DEFINE_GAMEPLAY_TAG(Input_Gameplay_Walk, "Input.Gameplay.Walk");
	UE_DEFINE_GAMEPLAY_TAG(Input_Gameplay_Strafe, "Input.Gameplay.Strafe");
	UE_DEFINE_GAMEPLAY_TAG(Input_Gameplay_Aim, "Input.Gameplay.Aim");
	UE_DEFINE_GAMEPLAY_TAG(Input_Gameplay_Traverse, "Input.Gameplay.Traverse");
	UE_DEFINE_GAMEPLAY_TAG(Input_Gameplay_Interact, "Input.Gameplay.Interact");
	UE_DEFINE_GAMEPLAY_TAG(Input_Gameplay_Combat, "Input.Gameplay.Combat");

	UE_DEFINE_GAMEPLAY_TAG(UI_Action_Confirm, "UI.Action.Confirm");
	UE_DEFINE_GAMEPLAY_TAG(UI_Action_Back, "UI.Action.Back");
}
