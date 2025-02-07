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
	
	/// 获取可游玩角色。
	/// @return 
	UFUNCTION(BlueprintPure, Category = "Blur ARPG Framework | Ability")
	ABlurPlayableCharacter* GetPlayableCharacterFromActorInfo();

	/// 获取玩家控制器。
	/// @return 
	UFUNCTION(BlueprintPure, Category = "Blur ARPG Framework | Ability")
	ABlurPlayerController* GetPlayerControllerFromActorInfo();

private:
	
	TWeakObjectPtr<ABlurPlayableCharacter> CachedBlurPlayableCharacter;
	TWeakObjectPtr<ABlurPlayerController> CachedBlurPlayerController;
};
