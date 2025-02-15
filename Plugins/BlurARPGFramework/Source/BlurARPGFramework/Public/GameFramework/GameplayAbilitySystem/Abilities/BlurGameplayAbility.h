// Blur Feng All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GameFramework/BlurGameplayTags.h"
#include "GameFramework/Types/BlurEnumTypes.h"

#include "BlurGameplayAbility.generated.h"

class UBlurCombatComponent;
class UBlurAbilityCombatComponent;
class UBlurCharacterUIComponent;
class UBlurPawnUIComponent;
class ABlurCharacterBase;
class IPawnUIInterface;
class UBlurAbilitySystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCheckCostOrCooldownDelegate, const bool, bAllow, const FGameplayTag, AbilityTag);

// 技能激活策略。
UENUM(BlueprintType)
enum class EAbilityActivationPolicy : uint8
{
	// 触发形式，通过输入或游戏事件触发的技能。
	OnTriggered,

	// 给定形式，立即触发一次性的效果，并立即移除。
	OnGiven,
};

// GA基类。
UCLASS()
class BLURARPGFRAMEWORK_API UBlurGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UBlurGameplayAbility();

protected:
	//~ Begin UGameplayAbility Interface.
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	virtual bool CheckCost(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	virtual bool CheckCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	//~ End UGameplayAbility Interface

	UFUNCTION()
	virtual void OnCheckCost(const bool bAllow, const FGameplayTag AbilityTag);
	UFUNCTION()
	virtual void OnCheckCooldown(const bool bAllow, const FGameplayTag AbilityTag);
	
public:
	// 当尝试触发技能，确认Cost是否足够时。
	UPROPERTY(BlueprintAssignable, Category = "Blur ARPG Framework | Ability", meta = (DisplayName = "On Check Cost"))
	FOnCheckCostOrCooldownDelegate OnCheckCostDelegate;

	// 当尝试触发技能，确认Cooldown是否结束。
	UPROPERTY(BlueprintAssignable, Category = "Blur ARPG Framework | Ability", meta = (DisplayName = "On Check Cool down"))
	FOnCheckCostOrCooldownDelegate OnCheckCooldownDelegate;

	// 条件确认方法。允许蓝图覆盖的方法。
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category = "Blur ARPG Framework | Ability")
	bool CheckConditionOnToggleableCancelAbility();

	// 条件确认方法。C++中实现的方法。
	virtual bool CheckConditionOnToggleableCancelAbility_Implementation();

protected:

	// 技能激活策略。根据策略不同会以不同的形式运行这个技能。
	UPROPERTY(EditDefaultsOnly, Category = "Blur ARPG Framework | Ability")
	EAbilityActivationPolicy AbilityActivationPolicy = EAbilityActivationPolicy::OnTriggered;

	/// 获取角色。
	/// @return 
	UFUNCTION(BlueprintPure, Category = "Blur ARPG Framework | Ability")
	ABlurCharacterBase* GetCharacterFromActorInfo();
	
	/// 获取战斗组件。
	/// @return 
	UFUNCTION(BlueprintPure, Category = "Blur ARPG Framework | Ability")
	UBlurCombatComponent* GetCombatComponentFromActorInfo();

	/// 获取技能战斗组件。
	/// @return 
	UFUNCTION(BlueprintPure, Category = "Blur ARPG Framework | Ability")
	UBlurAbilityCombatComponent* GetAbilityCombatComponentFromActorInfo();

	/// 获取PawnUI组件。
	/// @return 
	UFUNCTION(BlueprintPure, Category = "Blur ARPG Framework | Ability")
	UBlurPawnUIComponent* GetPawnUIComponentFromActorInfo();

	/// 获取角色UI组件。
	/// @return 
	UFUNCTION(BlueprintPure, Category = "Blur ARPG Framework | Ability")
	UBlurCharacterUIComponent* GetCharacterUIComponentFromActorInfo();

	/// 获取技能组件
	/// @return 
	UFUNCTION(BlueprintPure, Category = "Blur ARPG Framework | Ability")
	UBlurAbilitySystemComponent* GetBlurAbilitySystemComponentFromActorInfo() const;

	FActiveGameplayEffectHandle NativeApplyEffectSpecHandleTarget(AActor* TargetActor, const FGameplayEffectSpecHandle& InSpecHandle) const;

	/// 应用一个GE到目标Actor。
	/// @param TargetActor 
	/// @param InSpecHandle 
	/// @param OutSuccessType 
	/// @return 
	UFUNCTION(BlueprintCallable, Category = "Blur ARPG Framework | Ability", meta = (DisplayName = "Apply Gameplay Effect Spec Handle To Target Actor", ExpandEnumAsExecs = "OutSuccessType"))
	FActiveGameplayEffectHandle BP_ApplyEffectSpecHandleTarget(AActor* TargetActor, const FGameplayEffectSpecHandle& InSpecHandle, ESuccessType& OutSuccessType);

	/// 应用一个GE到FHitResult碰撞探测目标数组。
	/// 可以使用 BoxTrace 等探测方式来获得目标FHitResult。
	/// @param InSpecHandle GE说明句柄。
	/// @param TargetFlags 目标Flags。
	/// @param InHitReactEventTag 发送到目标的被击中事件Tag。无效时默认使用 BlurGameplayTags::Common_Event_HitReact。
	/// @param InHitResults Trace获得的Hit目标数组。
	UFUNCTION(BlueprintCallable, Category = "Blur ARPG Framework | Ability", meta = (Bitmask, BitmaskEnum = "ETeamAttitudeFlags"))
	void ApplyGameplayEffectSpecHandleToHitResults(const FGameplayEffectSpecHandle& InSpecHandle, const int32 TargetFlags, const FGameplayTag InHitReactEventTag, const TArray<FHitResult>& InHitResults);

	/// 应用一个GE到FHitResult碰撞探测目标。
	/// 可以使用 BoxTrace 等探测方式来获得目标FHitResult。
	/// @param InSpecHandle GE说明句柄。
	/// @param TargetFlags 目标Flags。
	/// @param InHitReactEventTag 发送到目标的被击中事件Tag。无效时默认使用 BlurGameplayTags::Common_Event_HitReact。
	/// @param InHitResult Trace获得的Hit目标。
	UFUNCTION(BlueprintCallable, Category = "Blur ARPG Framework | Ability")
	void ApplyGameplayEffectSpecHandleToHitResult(const FGameplayEffectSpecHandle& InSpecHandle, const int32 TargetFlags, const FGameplayTag InHitReactEventTag, const FHitResult& InHitResult);
	
	/// 创建GE查询句柄，用于施加GE。
	/// @param EffectClass 
	/// @return 
	UFUNCTION(BlueprintCallable, Category = "Blur ARPG Framework | Ability")
	FGameplayEffectSpecHandle MakeSpecHandle(const TSubclassOf<UGameplayEffect> EffectClass) const;

	/// 创建GE查询句柄，用于施加GE。
	/// @param EffectClass
	/// @param GameplayTag 查询用Tag。
	/// @param Magnitude 传递数值。
	/// @return 
	UFUNCTION(BlueprintCallable, Category = "Blur ARPG Framework | Ability")
	FGameplayEffectSpecHandle MakeSpecHandleSetByCallerMagnitude(const TSubclassOf<UGameplayEffect> EffectClass, const FGameplayTag GameplayTag, const float Magnitude) const;

	/// 创建伤害效果查询句柄。然后可以应用GE到目标上。
	/// @param EffectClass 
	/// @param InBaseDamage 基础伤害。
	/// @param InBaseDamageMultiplyCoefficient 基础伤害乘系数。
	/// @param DamageIncreaseCount 增伤计数。增伤幅度为 1 + DamageIncreaseCount * DamageIncreaseCoefficient。
	/// @param DamageIncreaseCoefficient 增伤系数。增伤幅度为 1 + DamageIncreaseCount * DamageIncreaseCoefficient。
	/// @return 
	UFUNCTION(BlueprintPure, Category = "Blur ARPG Framework | Ability", meta = (DisplayName = "Make Damage Effect Spec Handle",InBaseDamage = "10", InBaseDamageMultiplyCoefficient = "1", DamageIncreaseCount = "0", DamageIncreaseCoefficient = "0.15"))
	FGameplayEffectSpecHandle MakeDamageEffectSpecHandle(
		const TSubclassOf<UGameplayEffect> EffectClass, const float InBaseDamage, const float InBaseDamageMultiplyCoefficient, const int32 DamageIncreaseCount, const float DamageIncreaseCoefficient) const;

	/// 创建伤害效果查询句柄。然后可以应用GE到目标上。
	/// @param EffectClass 
	/// @param InBaseDamageScalableFloat 基础伤害可变值，根据AbilityLevel变化。
	/// @param InBaseDamageMultiplyCoefficient 基础伤害乘系数。
	/// @param DamageIncreaseCount 增伤计数。增伤幅度为 1 + DamageIncreaseCount * DamageIncreaseCoefficient。
	/// @param DamageIncreaseCoefficient 增伤系数。增伤幅度为 1 + DamageIncreaseCount * DamageIncreaseCoefficient。
	/// @return 
	UFUNCTION(BlueprintPure, Category = "Blur ARPG Framework | Ability", meta = (DisplayName = "Make Damage Effect Spec Handle", InBaseDamageMultiplyCoefficient = "1", DamageIncreaseCount = "0", DamageIncreaseCoefficient = "0.15"))
	FGameplayEffectSpecHandle MakeDamageEffectSpecHandleByScalableFloat(
	const TSubclassOf<UGameplayEffect> EffectClass, const FScalableFloat& InBaseDamageScalableFloat, const float InBaseDamageMultiplyCoefficient, const int32 DamageIncreaseCount, const float DamageIncreaseCoefficient) const;
	
	/// 获取可变float值根据自身AbilityLevel。
	/// @param InScalableFloat 
	/// @return 
	UFUNCTION(BlueprintPure, Category = "Blur ARPG Framework | Ability", meta = (CompactNodeTitle = "Get Value At Level"))
	float GetScalableFloatValueAtLevel(const FScalableFloat& InScalableFloat) const;

	/// 获取技能冷却剩余时间。
	/// @param InCooldownTag 冷却GETag。 
	/// @param TotalCooldownTime 总时间。
	/// @param RemainingCooldownTime 剩余时间。
	/// @return 
	UFUNCTION(BlueprintPure, Category = "Blur ARPG Framework | Ability")
	bool GetAbilityRemainingCooldownByTag(const FGameplayTag InCooldownTag, float& TotalCooldownTime, float& RemainingCooldownTime) const;

private:

	//Notes：TWeakObjectPtr 智能指针
	//TWeakObjectPtr 是 Unreal Engine 中一种智能指针类型，专门用来弱引用（Weak Reference）UObject 类型的对象。
	//它的设计目的是提供一种安全且高效的方式来引用可能会被销毁的对象，而不会阻止该对象被垃圾回收（Garbage Collection, GC）。因为它不会增加引用计数。
	//当 TWeakObjectPtr 所指向的对象被销毁时，它会自动变为失效状态。失效的 TWeakObjectPtr 不会造成崩溃（访问前需要手动检查有效性）。
	//由于不需要管理对象的生命周期，TWeakObjectPtr 的开销更低，非常适合缓存引用但不负责对象生命周期管理的场景。
	//应用场景：在这里，GameplayAbility是频繁创建和销毁的技能，技能中可能使用到 ABlurCharacterBase 但完全不想干扰到 ABlurCharacterBase 的生命周期。
	
	TWeakObjectPtr<ABlurCharacterBase> CachedBlurCharacterBase;
};
