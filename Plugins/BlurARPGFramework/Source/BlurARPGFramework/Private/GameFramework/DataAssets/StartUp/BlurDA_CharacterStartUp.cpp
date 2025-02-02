// Blur Feng All Rights Reserved.


#include "GameFramework/DataAssets/StartUp/BlurDA_CharacterStartup.h"

#include "GameFramework/BlurFunctionLibrary.h"
#include "GameFramework/GameplayAbilitySystem/BlurAbilitySystemComponent.h"

void UBlurDA_CharacterStartup::GiveToAbilitySystemComponent(UBlurAbilitySystemComponent* InAbilitySystemComponent, const int32 ApplyLevel)
{
	check(InAbilitySystemComponent);

	// Tips：这里只是赋予技能组件拥有者技能（而没有激活）。
	// 根据BLurGameplayAbility类中AbilityActivationPolicy的配置，在 OnGiveAbility 时GA会做不同的处理。

	// 初始化技能。
	GiveAbilities(ActivateOnGivenAbilities, InAbilitySystemComponent, ApplyLevel); // 在赋予技能后立即激活并清除。比如生成默认武器。
	GiveAbilities(ReactiveAbilities, InAbilitySystemComponent, ApplyLevel); // 反应式技能，比如死亡。

	// 应用启动时GameplayEffect到自身。比如设置最大血量和当前血量的GE。
	if(!StartUpGameplayEffects.IsEmpty())
	{
		for (const TSubclassOf<UGameplayEffect>& EffectClass : StartUpGameplayEffects)
		{
			if(!EffectClass) continue;

			// 通过 GetDefaultObject() 获取默认对象实例。此方法在UE中非常常用，避免我们自己多次创建相同的Object。
			const UGameplayEffect* EffectCDO = EffectClass->GetDefaultObject<UGameplayEffect>();
			InAbilitySystemComponent->ApplyGameplayEffectToSelf(
				EffectCDO,
				ApplyLevel,
				InAbilitySystemComponent->MakeEffectContext()
				);

			// // 其他添加GE的方式。
			// FGameplayEffectContextHandle EffectContext = InAbilitySystemComponent->MakeEffectContext();
			// EffectContext.AddSourceObject(this);
			// FGameplayEffectSpecHandle EffectSpecHandle = InAbilitySystemComponent->MakeOutgoingSpec(EffectClass, 1.f, EffectContext);
			// if (EffectSpecHandle.IsValid())
			// {
			// 	FActiveGameplayEffectHandle ActiveHandle = InAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
			// }
		}
	}

	// Tips：这里的代码实现和 GiveAbilities() 很像，但 GiveAbilities() 方法只赋予GameplayAbility。
	// 而这里我们通过 FGiveAbilitySet 来赋予 GameplayAbility 的同时，还设置了 InputTag。方便之后 UBlurEnhancedInputComponent 以及相关输入系统流程的使用。
	// 即使是敌人角色，我们也这么进行技能的赋予，方便游戏系统的扩展。比如“玩家使用技能来精神操控敌人”这样的需求时，即使是敌人的技能，也需要已经配置了InputTag。

	// 启动时赋予角色技能。
	for (const FGiveAbilitySet& AbilitySet : StartUpGiveAbilitySets)
	{
		if(!AbilitySet.IsValid()) continue;

		FGameplayAbilitySpec AbilitySpec =
			UBlurFunctionLibrary::GetGameplayAbilitySpec(AbilitySet.AbilityToGive, InAbilitySystemComponent->GetAvatarActor(), ApplyLevel, AbilitySet.InputTag);

		// Tips：这里赋予的技能是需要玩家输入进行使用的技能。
		// 给拥有者（技能组件）赋予技能，只有经过 GiveAbility() 赋予的技能才能激活使用。
		InAbilitySystemComponent->GiveAbility(AbilitySpec);
	}

	// 添加不受管理的松散Tag。
	// Tips：如果你像要Tag受到管理和同步，你应该使用GE来添加Tag。
	for (const FGameplayTag LooseTag : StartUpLooseGameplayTags)
	{
		InAbilitySystemComponent->AddLooseGameplayTag(LooseTag);
	}
}

void UBlurDA_CharacterStartup::GiveAbilities(const TArray<TSubclassOf<UBlurGameplayAbility>>& InAbilitiesToGive,
											  UBlurAbilitySystemComponent* InAbilitySystemComponent, const int32 ApplyLevel)
{
	if(InAbilitiesToGive.IsEmpty()) return;

	// 在启动时，将配置的GameplayAbility给到AvatarActor的AbilitySystemComponent。
	for(const TSubclassOf<UBlurGameplayAbility>& Ability : InAbilitiesToGive)
	{
		if(!Ability) continue;
		
		FGameplayAbilitySpec AbilitySpec =
			UBlurFunctionLibrary::GetGameplayAbilitySpec(Ability, InAbilitySystemComponent->GetAvatarActor(), ApplyLevel);

		// 给角色（技能组件）技能，只有经过GiveAbility()赋予的技能才能使用。
		InAbilitySystemComponent->GiveAbility(AbilitySpec);
	}
}

#if WITH_EDITOR
void UBlurDA_CharacterStartup::PostEditChangeProperty(struct FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif