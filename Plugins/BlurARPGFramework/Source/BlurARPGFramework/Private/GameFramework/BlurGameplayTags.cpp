// Blur Feng All Rights Reserved.


#include "GameFramework/BlurGameplayTags.h"

namespace BlurGameplayTags
{
	// Tips：Tag命名规则
	// 因为Tag在蓝图中使用时，Tag是按首字母顺序排列的。
	// 所以应当按项目需求规范命名规则。
	// 比如EquipAxe和UnequipAxe。我们将Axe作为开头的话，能根据具体对象分类排列。
	// 而用Equip和Unequip开头，则按照行为分类排列。

	UE_DEFINE_GAMEPLAY_TAG(None, "None");
	
	//** Input Tags **//
	UE_DEFINE_GAMEPLAY_TAG(Input_Move, "Input.Move");
	UE_DEFINE_GAMEPLAY_TAG(Input_Look, "Input.Look");
	UE_DEFINE_GAMEPLAY_TAG(Input_PickUp, "Input.PickUp");
	UE_DEFINE_GAMEPLAY_TAG(Input_SwitchTarget, "Input.SwitchTarget");
	UE_DEFINE_GAMEPLAY_TAG(Input_ResetView, "Input.ResetView");

	UE_DEFINE_GAMEPLAY_TAG(Input_Ability, "Input.Ability");
	UE_DEFINE_GAMEPLAY_TAG(Input_Ability_Toggleable, "Input.Ability.Toggleable");
	UE_DEFINE_GAMEPLAY_TAG(Input_Ability_MustBeHeld, "Input.Ability.MustBeHeld");
	UE_DEFINE_GAMEPLAY_TAG(Input_Ability_EnterCombat, "Input.Ability.EnterCombat");
	UE_DEFINE_GAMEPLAY_TAG(Input_Ability_ExitCombat, "Input.Ability.ExitCombat");

	//** Common Status **//
	UE_DEFINE_GAMEPLAY_TAG(Common_Status_Blocking, "Common.Status.Blocking");
	UE_DEFINE_GAMEPLAY_TAG(Common_Status_Unblockable, "Common.Status.Unblockable");
	UE_DEFINE_GAMEPLAY_TAG(Common_Status_Health_Full, "Common.Status.Health.Full");
	UE_DEFINE_GAMEPLAY_TAG(Common_Status_Health_Empty, "Common.Status.Health.Empty");
	UE_DEFINE_GAMEPLAY_TAG(Common_Status_Stamina_Full, "Common.Status.Stamina.Full");
	UE_DEFINE_GAMEPLAY_TAG(Common_Status_Stamina_Empty, "Common.Status.Stamina.Empty");
	UE_DEFINE_GAMEPLAY_TAG(Common_Status_Rage_Full, "Common.Status.Rage.Full");
	UE_DEFINE_GAMEPLAY_TAG(Common_Status_Rage_Empty, "Common.Status.Rage.Empty");
	UE_DEFINE_GAMEPLAY_TAG(Common_Status_InCombat, "Common.Status.InCombat");
	
	
	UE_DEFINE_GAMEPLAY_TAG(Common_Status_Dead, "Common.Status.Dead");

	//** Common Event **//
	UE_DEFINE_GAMEPLAY_TAG(Common_Event_PickUp, "Common.Event.PickUp");
	UE_DEFINE_GAMEPLAY_TAG(Common_Event_SwitchTarget_Triggered, "Common.Event.SwitchTarget.Triggered");
	UE_DEFINE_GAMEPLAY_TAG(Common_Event_SwitchTarget_Completed, "Common.Event.SwitchTarget.Completed");
	UE_DEFINE_GAMEPLAY_TAG(Common_Event_ResetView, "Common.Event.ResetView");
	UE_DEFINE_GAMEPLAY_TAG(Common_Event_Montage_EnterCombat, "Common.Event.Montage.EnterCombat");
	UE_DEFINE_GAMEPLAY_TAG(Common_Event_Montage_ExitCombat, "Common.Event.Montage.ExitCombat");
	
	UE_DEFINE_GAMEPLAY_TAG(Common_Event_MeleeHit, "Common.Event.MeleeHit");
	UE_DEFINE_GAMEPLAY_TAG(Common_Event_MeleePulled, "Common.Event.MeleePulled");
	UE_DEFINE_GAMEPLAY_TAG(Common_Event_SuccessfulBlock, "Common.Event.SuccessfulBlock");
	UE_DEFINE_GAMEPLAY_TAG(Common_Event_Blocking, "Common.Event.Blocking");
	UE_DEFINE_GAMEPLAY_TAG(Common_Event_Unblockable, "Common.Event.Unblockable");
	UE_DEFINE_GAMEPLAY_TAG(Common_Event_HitReact, "Common.Event.HitReact");

	UE_DEFINE_GAMEPLAY_TAG(Common_Event_RepeatInputWhenAbilityActive, "Common.Event.RepeatInputWhenAbilityActive");
	UE_DEFINE_GAMEPLAY_TAG(Common_Event_Combo_WaitInput, "Common.Event.Combo.WaitInput");
	UE_DEFINE_GAMEPLAY_TAG(Common_Event_Combo_StartNextAttack, "Common.Event.Combo.StartNextAttack");
	UE_DEFINE_GAMEPLAY_TAG(Common_Event_Combo_Reset, "Common.Event.Combo.Reset");

	//** Common Ability **//
	UE_DEFINE_GAMEPLAY_TAG(Common_Ability_Cooldown, "Common.Ability.Cooldown");

	//** GEExecCalc **//
	UE_DEFINE_GAMEPLAY_TAG(GEExecCalc_SetByCaller_Attack_BaseDamage, "GEExecCalc.SetByCaller.Attack.BaseDamage");
	UE_DEFINE_GAMEPLAY_TAG(GEExecCalc_SetByCaller_Attack_BaseDamageMultiplyCoefficient, "GEExecCalc.SetByCaller.Attack.BaseDamage");
	UE_DEFINE_GAMEPLAY_TAG(GEExecCalc_SetByCaller_Attack_DamageIncreaseCount, "GEExecCalc.SetByCaller.Attack.BaseDamage");
	UE_DEFINE_GAMEPLAY_TAG(GEExecCalc_SetByCaller_Attack_DamageIncreaseCoefficient, "GEExecCalc.SetByCaller.Attack.BaseDamage");

	//** GameData **//
	UE_DEFINE_GAMEPLAY_TAG(GameData_Level, "GameData.Level");
	
}