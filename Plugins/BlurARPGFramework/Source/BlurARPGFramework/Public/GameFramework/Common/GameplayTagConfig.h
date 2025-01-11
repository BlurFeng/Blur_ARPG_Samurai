#pragma once

// 这里是ARPGFramework插件提供的功能中。一些可以按项目需求配置的Tag。
// 这些Tag在插件提供的功能中发挥作用。必须正确的配置这些Tag和你项目的Tag列表中要求的一致。

struct FGameplayTagConfig
{
	// 切换形式技能的输入Tag的父级Tag。用于判断输入技能的操作方式。
	static constexpr const char* Tag_InputToggleableParentsTag = "Input.Toggleable";

	// 持续按住形式技能的输入Tag的父级Tag。用于判断输入技能的操作方式。
	static constexpr const char* Tag_InputMustBeHeldParentsTag = "Input.MustBeHeld";
	
	static constexpr const char* Tag_CommonEventMeleeHit = "Common.Event.MeleeHit";
	static constexpr const char* Tag_CommonEventMeleePulled = "Common.Event.MeleePulled";
	static constexpr const char* Tag_CommonEventSuccessfulBlock = "Common.Event.SuccessfulBlock";
	static constexpr const char* Tag_CommonStatusBlocking = "Common.Status.Blocking";
	static constexpr const char* Tag_CommonStatusUnblockable = "Common.Status.Unblockable";

	// 插件提供的 UBlurGameplayAbilityBase::MakeDamageEffectSpecHandle 方法需要传递的参数。
	// 这些参数可以为 UGEExecCalc_DamageTaken 伤害计算方法提供需要的参数。
	// 你也可以按自己的需求像这样进行参数传递。或者构建自己的 MakeDamageEffectSpecHandle 和 UGameplayEffectExecutionCalculation 子类用于计算数值。
	static constexpr const char* MakeDamageEffectSpecHandle_SetByCaller_BaseDamage = "GEExecCalc.SetByCaller.Attack.BaseDamage";
	static constexpr const char* MakeDamageEffectSpecHandle_SetByCaller_BaseDamageMultiplyCoefficient = "GEExecCalc.SetByCaller.Attack.BaseDamage.MultiplyCoefficient";
	static constexpr const char* MakeDamageEffectSpecHandle_SetByCaller_DamageIncreaseCount = "GEExecCalc.SetByCaller.Attack.DamageIncreaseCount";
	static constexpr const char* MakeDamageEffectSpecHandle_SetByCaller_DamageIncreaseCoefficient = "GEExecCalc.SetByCaller.Attack.DamageIncreaseCoefficient";
};
