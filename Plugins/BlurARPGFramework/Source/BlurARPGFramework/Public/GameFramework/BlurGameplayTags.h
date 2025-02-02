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
	BLURARPGFRAMEWORK_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_PickUp);
	BLURARPGFRAMEWORK_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_SwitchTarget);
	
	BLURARPGFRAMEWORK_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_Toggleable);
	BLURARPGFRAMEWORK_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Input_MustBeHeld);

	//** Common Status **//
	BLURARPGFRAMEWORK_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Common_Status_Blocking);
	BLURARPGFRAMEWORK_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Common_Status_Unblockable);
	BLURARPGFRAMEWORK_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Common_Status_Health_Full);
	BLURARPGFRAMEWORK_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Common_Status_Health_Empty);
	BLURARPGFRAMEWORK_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Common_Status_Stamina_Full);
	BLURARPGFRAMEWORK_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Common_Status_Stamina_Empty);
	BLURARPGFRAMEWORK_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Common_Status_Rage_Full);
	BLURARPGFRAMEWORK_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Common_Status_Rage_Empty);

	BLURARPGFRAMEWORK_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Common_Status_Dead);
	
	//** Common Event **//
	BLURARPGFRAMEWORK_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Common_Event_PickUp);
	BLURARPGFRAMEWORK_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Common_Event_SwitchTarget_Left);
	BLURARPGFRAMEWORK_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Common_Event_SwitchTarget_Right);
	
	BLURARPGFRAMEWORK_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Common_Event_MeleeHit);
	BLURARPGFRAMEWORK_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Common_Event_MeleePulled);
	BLURARPGFRAMEWORK_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Common_Event_SuccessfulBlock);
	BLURARPGFRAMEWORK_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Common_Event_Blocking);
	BLURARPGFRAMEWORK_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Common_Event_Unblockable);

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
