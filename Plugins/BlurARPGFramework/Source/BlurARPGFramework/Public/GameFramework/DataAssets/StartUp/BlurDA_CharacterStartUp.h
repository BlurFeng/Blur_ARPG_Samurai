// Blur Feng All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"

#include "BlurDA_CharacterStartup.generated.h"

struct FGiveAbilitySet;
class UBlurAbilitySystemComponent;
class UGameplayEffect;
class UBlurGameplayAbility;

// 角色启动数据资源。用于配置角色启动时的一些配置，比如技能和GE。
UCLASS()
class BLURARPGFRAMEWORK_API UBlurDA_CharacterStartup : public UDataAsset
{
	GENERATED_BODY()

public:
	virtual void GiveToAbilitySystemComponent(UBlurAbilitySystemComponent* InAbilitySystemComponent, const int32 ApplyLevel = 1);

protected:
	// 角色启动时技能设置组。用于在启动时赋予角色技能，这些技能是主动或被动技能，在必要时激活。他们都是游玩时使用到的技能，有复杂的状态机制。
	UPROPERTY(EditDefaultsOnly, category = "Blur ARPG Framework | Startup Data", meta = (TitleProperty = "InputTag"))
	TArray<FGiveAbilitySet> StartUpGiveAbilitySets;

	// 立即激活技能组。在启动后立刻赋予技能并激活，然后从技能将被移除。比如生成默认武器。
	UPROPERTY(EditDefaultsOnly, category = "Blur ARPG Framework | Startup Data")
	TArray<TSubclassOf<UBlurGameplayAbility>> ActivateOnGivenAbilities;

	// 反应触发式技能。在特定条件下触发的技能。比如死亡。
	UPROPERTY(EditDefaultsOnly, category = "Blur ARPG Framework | Startup Data")
	TArray<TSubclassOf<UBlurGameplayAbility>> ReactiveAbilities;

	// 启动时GE。在启动时直接应用到自身。比如设置最大血量和当前血量或持续恢复体力等。注意，如果GE本身有依赖关系，请注意配置的前后顺序。我们从数组的0位开始应用GE到自身。
	UPROPERTY(EditDefaultsOnly, category = "Blur ARPG Framework | Startup Data")
	TArray<TSubclassOf<UGameplayEffect>> StartUpGameplayEffects;

	// 不受管理的松散Tag。这些Tag不受到GE管理，也不能在服务器和客户端之间同步。
	// 很适合用来添加一些永久的标记Tag，比如角色的身份 Identity Tag。
	UPROPERTY(EditDefaultsOnly, category = "Blur ARPG Framework | Startup Data")
	TArray<FGameplayTag> StartUpLooseGameplayTags;

	/// 赋予技能
	/// @param InAbilitiesToGive 
	/// @param InAbilitySystemComponent 
	/// @param ApplyLevel 
	static void GiveAbilities(const TArray<TSubclassOf<UBlurGameplayAbility>>& InAbilitiesToGive, UBlurAbilitySystemComponent* InAbilitySystemComponent, const int32 ApplyLevel = 1);

#if WITH_EDITOR
	virtual void PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
