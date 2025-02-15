// Blur Feng All Rights Reserved.

//Notes:
//我们通过C++来创建GameplayTags。而不是通过UE的主菜单>Project Settings>GameplayTags>Gameplay Tag List的Manage Gameplay Tags编辑器工具进行创建。
//这样可以在C++中更方便的访问GameplayTags而不用担心拼写错误。
//UE_DEFINE_GAMEPLAY_TAG(InputTag_Move, "InputTag.Move")定义Tag时，InputTag_Move是用于C++访问的名称。InputTag.Move是显示名称。

#pragma once

#include "NativeGameplayTags.h"

namespace BlurGameplayTags
{
	// Tips：这里是 BlurARPGFramework 插件使用到的Tag。你的项目也可以按这样的命名方式注入自己的Tag。
	// 已经存在的Tag在你的项目中不用在重复的注入。但如果重复注入了，实际上Engine也只会认为他们是一样的Tag。

	// Tips：在一些配置数据中，我们使用了 meta = (Categories = "Ability.Cooldown") 的方式来限制可选的 Tag父层级。
	// 你的项目也应当按照这样的Tag层级来添加自己项目的Tag。否则可能需要修改这部分代码。
	
	BLURARPGFRAMEWORK_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(None);
	
	//** Input Tags **//
	BLURARPGFRAMEWORK_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Move);
	BLURARPGFRAMEWORK_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Look);
	BLURARPGFRAMEWORK_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_PickUp); // 拾取物品。
	BLURARPGFRAMEWORK_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_SwitchTarget); // 切换目标。可用于锁定敌人时进行目标切换等功能。
	BLURARPGFRAMEWORK_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_ResetView); // 重置玩家当前相机视角。

	// 技能输入Tag的父级Tag。项目添加此Tag的子Tag作为技能输入Tag。
	BLURARPGFRAMEWORK_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Ability);
	BLURARPGFRAMEWORK_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Ability_Toggleable); // 切换形式。在激活和取消之间切换。
	BLURARPGFRAMEWORK_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Ability_MustBeHeld); // 持续按住形式。按下时激活，抬起时取消。

	//** Common Status **//
	BLURARPGFRAMEWORK_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Common_Status_Blocking); // 格挡状态，可以防御敌人的进攻。
	BLURARPGFRAMEWORK_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Common_Status_Unblockable); // 不可阻挡状态，拥有此状态的觉得进行的攻击不能被格挡。
	BLURARPGFRAMEWORK_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Common_Status_Health_Full);
	BLURARPGFRAMEWORK_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Common_Status_Health_Empty);
	BLURARPGFRAMEWORK_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Common_Status_Stamina_Full);
	BLURARPGFRAMEWORK_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Common_Status_Stamina_Empty);
	BLURARPGFRAMEWORK_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Common_Status_Rage_Full);
	BLURARPGFRAMEWORK_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Common_Status_Rage_Empty);
	BLURARPGFRAMEWORK_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Common_Status_InCombat); // 正在战斗中。当 BlurCombatComponent 组件使用武器进入战斗状态时添加。
	
	BLURARPGFRAMEWORK_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Common_Status_Dead); // 死亡状态。
	
	//** Common Event **//
	BLURARPGFRAMEWORK_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Common_Event_PickUp);
	BLURARPGFRAMEWORK_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Common_Event_SwitchTarget_Triggered); // 切换目标输入触发时事件。监听鼠标移动输入量。
	BLURARPGFRAMEWORK_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Common_Event_SwitchTarget_Completed); // 切换目标输入完成时事件。判断鼠标移动输入量是否能够触发切换目标。
	BLURARPGFRAMEWORK_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Common_Event_ResetView); // 触发重置视口技能。
	
	BLURARPGFRAMEWORK_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Common_Event_MeleeHit);
	BLURARPGFRAMEWORK_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Common_Event_MeleePulled);
	BLURARPGFRAMEWORK_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Common_Event_SuccessfulBlock);
	BLURARPGFRAMEWORK_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Common_Event_Blocking);
	BLURARPGFRAMEWORK_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Common_Event_Unblockable);
	BLURARPGFRAMEWORK_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Common_Event_HitReact);

	BLURARPGFRAMEWORK_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Common_Event_RepeatInputWhenAbilityActive);
	BLURARPGFRAMEWORK_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Common_Event_Combo_WaitInput);
	BLURARPGFRAMEWORK_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Common_Event_Combo_StartNextAttack);
	BLURARPGFRAMEWORK_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Common_Event_Combo_Reset);
	

	//** Common Ability **//
	BLURARPGFRAMEWORK_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Common_Ability_Cooldown);
	
	//** GEExecCalc **//
	BLURARPGFRAMEWORK_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GEExecCalc_SetByCaller_Attack_BaseDamage);
	BLURARPGFRAMEWORK_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GEExecCalc_SetByCaller_Attack_BaseDamageMultiplyCoefficient);
	BLURARPGFRAMEWORK_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GEExecCalc_SetByCaller_Attack_DamageIncreaseCount);
	BLURARPGFRAMEWORK_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GEExecCalc_SetByCaller_Attack_DamageIncreaseCoefficient);

	//** GameData **//
	BLURARPGFRAMEWORK_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameData_Level);
	
}
