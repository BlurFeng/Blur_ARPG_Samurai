// Blur Feng All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameplayAbilitySystem/Abilities/BlurGameplayAbility.h"
#include "BlurPlayableCharacterGameplayAbility.generated.h"

class ABlurPlayerController;
class ABlurPlayableCharacter;

// 可游玩角色技能基类。
UCLASS()
class BLURARPGFRAMEWORK_API UBlurPlayableCharacterGameplayAbility : public UBlurGameplayAbility
{
	GENERATED_BODY()

public:

	virtual void OnCheckCost(const bool bAllow, const FGameplayTag AbilityTag) override;
	virtual void OnCheckCooldown(const bool bAllow, const FGameplayTag AbilityTag) override;
	
	/// 获取英雄角色
	/// @return 
	UFUNCTION(BlueprintPure, Category = "Warrior|Ability")
	ABlurPlayableCharacter* GetPlayableCharacterFromActorInfo();

	/// 获取英雄控制器
	/// @return 
	UFUNCTION(BlueprintPure, Category = "Warrior|Ability")
	ABlurPlayerController* GetPlayerControllerFromActorInfo();

private:
	
	TWeakObjectPtr<ABlurPlayableCharacter> CachedWarriorHeroCharacter;
	TWeakObjectPtr<ABlurPlayerController> CachedWarriorHeroController;
};
