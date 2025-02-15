// Blur Feng All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Components/Combat/BlurCombatComponent.h"
#include "BlurAbilityCombatComponent.generated.h"

class ABlurAbilityWeapon;

UCLASS()
class BLURARPGFRAMEWORK_API UBlurAbilityCombatComponent : public UBlurCombatComponent
{
	GENERATED_BODY()

public:
	/// 获取持有的武器，根据Tag。
	/// @param InWeaponTag 武器Tag。
	/// @return 
	UFUNCTION(BlueprintCallable, Category = "Blur ARPG Framework | Ability Combat")
	ABlurAbilityWeapon* GetCarriedAbilityWeaponByTag(FGameplayTag InWeaponTag) const;

	/// 获取当前装备的武器。
	/// @return 
	UFUNCTION(BlueprintCallable, Category = "Blur ARPG Framework | Ability Combat")
	ABlurAbilityWeapon* GetCurrentEquippedAbilityWeapon() const;

	/// 获取当前装备武器的伤害。根据Level。
	/// @param InLevel 武器伤害Level。武器基础伤害实际上配置了曲线，根据输入Level获取相应的基础伤害。
	/// @return 武器基础伤害。
	UFUNCTION(BlueprintCallable, Category = "Blur ARPG Framework | Ability Combat")
	float GetCurrentEquippedAbilityWeaponDamageAtLevel(const float InLevel) const;

	//** 回调 **//
	virtual void OnMeleeHitTargetActor(AActor* HitActor) override; // 当武器命中目标时。
	virtual void OnMeleePulledFromTargetActor(AActor* HitActor) override; // 当武器离开目标时。
};
