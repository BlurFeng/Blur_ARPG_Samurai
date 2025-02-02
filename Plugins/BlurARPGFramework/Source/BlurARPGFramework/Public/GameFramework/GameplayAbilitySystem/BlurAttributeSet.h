// Blur Feng All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "BlurAttributeSet.generated.h"

// 我们使用宏帮我们快速定义一个属性对应的初始化、设置、获取方法。
#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

class IBlurPawnUIInterface;

UCLASS()
class BLURARPGFRAMEWORK_API UBlurAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	UBlurAttributeSet();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;

	// 生命。
	UPROPERTY(BlueprintReadOnly, Category = "Attribute Set", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UBlurAttributeSet, Health)

	UPROPERTY(BlueprintReadOnly, Category = "Attribute Set", ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UBlurAttributeSet, MaxHealth)

	// 耐力。
	UPROPERTY(BlueprintReadOnly, Category = "Attribute Set", ReplicatedUsing = OnRep_Stamina)
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UBlurAttributeSet, Stamina)

	UPROPERTY(BlueprintReadOnly, Category = "Attribute Set", ReplicatedUsing = OnRep_MaxStamina)
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UBlurAttributeSet, MaxStamina)

	// 怒气。
	UPROPERTY(BlueprintReadOnly, Category = "Attribute Set", ReplicatedUsing = OnRep_Rage)
	FGameplayAttributeData Rage;
	ATTRIBUTE_ACCESSORS(UBlurAttributeSet, Rage)

	UPROPERTY(BlueprintReadOnly, Category = "Attribute Set", ReplicatedUsing = OnRep_MaxRage)
	FGameplayAttributeData MaxRage;
	ATTRIBUTE_ACCESSORS(UBlurAttributeSet, MaxRage)

	// 经验值。
	UPROPERTY(BlueprintReadOnly, Category = "Attribute Set", ReplicatedUsing = OnRep_Rage)
	FGameplayAttributeData ExperienceValue;
	ATTRIBUTE_ACCESSORS(UBlurAttributeSet, ExperienceValue)

	UPROPERTY(BlueprintReadOnly, Category = "Attribute Set", ReplicatedUsing = OnRep_MaxRage)
	FGameplayAttributeData MaxExperienceValue;
	ATTRIBUTE_ACCESSORS(UBlurAttributeSet, MaxExperienceValue)

	// 攻击力。
	UPROPERTY(BlueprintReadOnly, Category = "Attribute Set")
	FGameplayAttributeData AttackPower;
	ATTRIBUTE_ACCESSORS(UBlurAttributeSet, AttackPower)

	// 防御力。
	UPROPERTY(BlueprintReadOnly, Category = "Attribute Set")
	FGameplayAttributeData DefensePower;
	ATTRIBUTE_ACCESSORS(UBlurAttributeSet, DefensePower)

	// 伤害接收数值缓存。
	// 用于暂时存储受到的伤害，最终作用于CurrentHealth。方便我们在结算伤害前再进行其他计算。
	// Tips：此值是Override的并不累加。理论上来说一帧之内不会有多个伤害计算 override DamageTaken的值。如果出问题需要扩展修改相关逻辑。
	UPROPERTY(BlueprintReadOnly, Category = "Attribute Set")
	FGameplayAttributeData DamageTaken;
	ATTRIBUTE_ACCESSORS(UBlurAttributeSet, DamageTaken)

	// 从受到伤害获取的怒气值系数。获得怒气值 = DamageTaken * GainRageByDamageTakenCoefficient。
	UPROPERTY(BlueprintReadOnly, Category = "Attribute Set")
	FGameplayAttributeData GainRageByDamageTakenCoefficient;
	ATTRIBUTE_ACCESSORS(UBlurAttributeSet, GainRageByDamageTakenCoefficient)

	UPROPERTY(BlueprintReadOnly, Category = "Attribute Set")
	FGameplayAttributeData GainRageByDamageTakenCached;
	ATTRIBUTE_ACCESSORS(UBlurAttributeSet, GainRageByDamageTakenCached)

protected:
	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);

	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);

	UFUNCTION()
	virtual void OnRep_Stamina(const FGameplayAttributeData& OldStamina);

	UFUNCTION()
	virtual void OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina);

	UFUNCTION()
	virtual void OnRep_Rage(const FGameplayAttributeData& OldRage);

	UFUNCTION()
	virtual void OnRep_MaxRage(const FGameplayAttributeData& OldMaxRage);

private:
	// UI通知接口缓存，用于通知UI更新显示属性。
	TWeakInterfacePtr<IBlurPawnUIInterface> CachedPawnUIInterface;

	// 当最大值变化时，同时调整对应的当前值。
	void AdjustAttributeForMaxChange(
		const FGameplayAttributeData& HandleAttribute, const FGameplayAttributeData& MaxAttribute, const float NewMaxValue, const FGameplayAttribute& HandleAttributeProperty) const;

	void SetRageCheck(const struct FGameplayEffectModCallbackData& Data, float NewCurrentVal);
};
