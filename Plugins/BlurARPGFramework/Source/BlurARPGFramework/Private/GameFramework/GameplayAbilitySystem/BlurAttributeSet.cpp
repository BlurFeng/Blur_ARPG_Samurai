// Blur Feng All Rights Reserved.


#include "GameFramework/GameplayAbilitySystem/BlurAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "GameFramework/BlurFunctionLibrary.h"
#include "GameFramework/BlurGameplayTags.h"
#include "GameFramework/Characters/BlurCharacterBase.h"
#include "GameFramework/Interfaces/BlurPawnUIInterface.h"

#include "GameFramework/Common/BlurDebugHelper.h"
#include "GameFramework/Components/UI/BlurCharacterUIComponent.h"
#include "Net/UnrealNetwork.h"

UBlurAttributeSet::UBlurAttributeSet()
{
}

void UBlurAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UBlurAttributeSet, Health);
	DOREPLIFETIME(UBlurAttributeSet, MaxHealth);
	DOREPLIFETIME(UBlurAttributeSet, Stamina);
	DOREPLIFETIME(UBlurAttributeSet, MaxStamina);
	DOREPLIFETIME(UBlurAttributeSet, Rage);
	DOREPLIFETIME(UBlurAttributeSet, MaxRage);
	DOREPLIFETIME(UBlurAttributeSet, ExperienceValue);
	DOREPLIFETIME(UBlurAttributeSet, MaxExperienceValue);
}

void UBlurAttributeSet::PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	float DeltaValue = 0.f;
	if (Data.EvaluatedData.ModifierOp == EGameplayModOp::Additive)
	{
		DeltaValue = Data.EvaluatedData.Magnitude;
	}
	AActor* TargetActor = nullptr;
	ABlurCharacterBase* CharacterBase = nullptr;
	if (Data.Target.AbilityActorInfo.IsValid() && Data.Target.AbilityActorInfo->AvatarActor.IsValid())
	{
		TargetActor = Data.Target.AbilityActorInfo->AvatarActor.Get();
		CharacterBase = Cast<ABlurCharacterBase>(TargetActor);
	}

	// 获取人物UI接口。
	if(!CachedPawnUIInterface.IsValid())
	{
		// 使用弱指针缓存UI接口。
		CachedPawnUIInterface = TWeakInterfacePtr<IBlurPawnUIInterface>(Data.Target.GetAvatarActor());
		// CachedPawnUIInterface = Cast<IPawnUIInterface>(Data.Target.GetAvatarActor()); //等价方法。
	}
	checkf(CachedPawnUIInterface.IsValid(), TEXT("%s didn't implement IPawnUIInterface."), *Data.Target.GetAvatarActor()->GetActorNameOrLabel())

	// 获取人物UI组件。
	const UBlurPawnUIComponent* PawnUIComponent = CachedPawnUIInterface->GetPawnUIComponent();
	checkf(PawnUIComponent, TEXT("Couldn't extract a PawnUIComponent from %s .") ,*Data.Target.GetAvatarActor()->GetActorNameOrLabel())
	
	// 生命值。
	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		const float NewCurrentHealth = FMath::Clamp(GetHealth(), 0.f, GetMaxHealth());
		SetHealth(NewCurrentHealth);

		// 生命值达到最大。
		if (GetHealth() >= GetMaxHealth())
		{
			UBlurFunctionLibrary::AddGameplayTagToActorIfNone(Data.Target.GetAvatarActor(), BlurGameplayTags::Common_Status_Health_Full);
		}
		// 生命值变为空。
		else if (GetHealth() <= 0.f)
		{
			UBlurFunctionLibrary::AddGameplayTagToActorIfNone(Data.Target.GetAvatarActor(), BlurGameplayTags::Common_Status_Health_Empty);
		}
		// 移除生命值最大和生命值空Tag。
		else
		{
			UBlurFunctionLibrary::RemoveGameplayTagFromActorIfFound(Data.Target.GetAvatarActor(),BlurGameplayTags::Common_Status_Health_Full);
			UBlurFunctionLibrary::RemoveGameplayTagFromActorIfFound(Data.Target.GetAvatarActor(),BlurGameplayTags::Common_Status_Health_Empty);
		}

		// UI更新广播。
		PawnUIComponent->OnHealthChanged.Broadcast(GetHealth() / GetMaxHealth());
	}
	// 耐力值。
	else if (Data.EvaluatedData.Attribute == GetStaminaAttribute())
	{
		const float NewCurrentStamina = FMath::Clamp(GetStamina(), 0.f, GetMaxStamina());
		SetStamina(NewCurrentStamina);

		// 耐力值达到最大。
		if (GetStamina() >= GetMaxStamina())
		{
			UBlurFunctionLibrary::AddGameplayTagToActorIfNone(Data.Target.GetAvatarActor(), BlurGameplayTags::Common_Status_Stamina_Full);
		}
		// 耐力值变为空。
		else if (GetStamina() <= 0.f)
		{
			UBlurFunctionLibrary::AddGameplayTagToActorIfNone(Data.Target.GetAvatarActor(), BlurGameplayTags::Common_Status_Stamina_Empty);
		}
		// 移除耐力值最大和耐力值空Tag。
		else
		{
			UBlurFunctionLibrary::RemoveGameplayTagFromActorIfFound(Data.Target.GetAvatarActor(),BlurGameplayTags::Common_Status_Stamina_Full);
			UBlurFunctionLibrary::RemoveGameplayTagFromActorIfFound(Data.Target.GetAvatarActor(),BlurGameplayTags::Common_Status_Stamina_Empty);
		}
		
		// UI更新广播。
		if(const UBlurCharacterUIComponent* CharacterUIComponent = CachedPawnUIInterface->GetCharacterUIComponent())
		{
			CharacterUIComponent->OnStaminaChanged.Broadcast(GetStamina() / GetMaxStamina());
		}
	}
	// 愤怒值。
	else if (Data.EvaluatedData.Attribute == GetRageAttribute())
	{
		SetRageCheck(Data, GetRage());
	}
	// 经验值。
	else if (Data.EvaluatedData.Attribute == GetExperienceValueAttribute())
	{
		const float NewCurrentExp = FMath::Clamp(GetExperienceValue(), 0.f, GetMaxExperienceValue());
		SetStamina(NewCurrentExp);

		// 升级。
		if (NewCurrentExp >= GetMaxExperienceValue())
		{
			// TODO: 升级。置零当前经验值，更新最大经验值。
		}
	}
	// 伤害承受值。
	else if (Data.EvaluatedData.Attribute == GetDamageTakenAttribute())
	{
		// 将 承受伤害值 应用到 当前生命值。
		const float OldHealth = GetHealth();
		const float DamageDone = GetDamageTaken();
		const float NewCurrentHealth = FMath::Clamp(OldHealth - DamageDone, 0.f, GetMaxHealth());
		SetHealth(NewCurrentHealth);

		// const FString DebugString = FString::Printf(TEXT("Old Health: %f, Damage Done: %f, New Current Health: %f"), OldHealth, DamageDone, NewCurrentHealth);
		// Debug::Print(DebugString, FColor::Green);

		// UI更新广播。
		PawnUIComponent->OnHealthChanged.Broadcast(GetHealth()/GetMaxHealth());

		// 触发死亡。
		if (GetHealth() == 0.f)
		{
			// Debug::Print(TEXT("Need to Death."), FColor::Red);
			// 通过给角色添加 Shared_Status_Dead 来触发 GA_Enemy_Death_Base 死亡技能。在死亡技能中，我们配置了此GA的触发方式为添加 Shared_Status_Dead Tag时。
			UBlurFunctionLibrary::AddGameplayTagToActorIfNone(Data.Target.GetAvatarActor(), BlurGameplayTags::Common_Status_Dead);
		}

		// 不清空此属性，作为缓存供其他计算或逻辑使用。
		// SetDamageTaken(0.f);
	}
	else if (Data.EvaluatedData.Attribute == GetGainRageByDamageTakenCachedAttribute())
	{
		// 将从承受伤害获得的怒气 应用到 当前怒气。
		const float OldRage = GetRage();
		const float GainRage = GetGainRageByDamageTakenCached();
		SetRageCheck(Data, OldRage + GainRage);
	}
}

void UBlurAttributeSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	// Tips: 初始Max值为0.通过GE设置Max。所以一开始设置Max时也会将对应的Current从0调整到和Max相等。

	if (Attribute == GetMaxHealthAttribute())
	{
		AdjustAttributeForMaxChange(Health, MaxHealth, NewValue, GetHealthAttribute());
	}
	else if (Attribute == GetMaxStaminaAttribute())
	{
		AdjustAttributeForMaxChange(Stamina, MaxStamina, NewValue, GetStaminaAttribute());
	}
	else if (Attribute == GetMaxRageAttribute())
	{
		AdjustAttributeForMaxChange(Rage, MaxRage, NewValue, GetRageAttribute());
	}
}

void UBlurAttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBlurAttributeSet, Health, OldHealth);
}

void UBlurAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBlurAttributeSet, MaxHealth, OldMaxHealth);
}

void UBlurAttributeSet::OnRep_Stamina(const FGameplayAttributeData& OldStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBlurAttributeSet, Stamina, OldStamina);
}

void UBlurAttributeSet::OnRep_MaxStamina(const FGameplayAttributeData& OldMaxStamina)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBlurAttributeSet, MaxStamina, OldMaxStamina);
}

void UBlurAttributeSet::OnRep_Rage(const FGameplayAttributeData& OldRage)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBlurAttributeSet, Rage, OldRage);
}

void UBlurAttributeSet::OnRep_MaxRage(const FGameplayAttributeData& OldMaxRage)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UBlurAttributeSet, MaxRage, OldMaxRage);
}

void UBlurAttributeSet::AdjustAttributeForMaxChange(const FGameplayAttributeData& HandleAttribute,
	const FGameplayAttributeData& MaxAttribute, const float NewMaxValue, const FGameplayAttribute& HandleAttributeProperty) const
{
	UAbilitySystemComponent* AbilitySystemComponent = GetOwningAbilitySystemComponent();
	const float CurrentMaxValue = MaxAttribute.GetCurrentValue();
	if (!FMath::IsNearlyEqual(CurrentMaxValue, NewMaxValue) && AbilitySystemComponent)
	{
		const float CurrentValue = HandleAttribute.GetCurrentValue();
		const float NewDelta = CurrentMaxValue > 0.f ? (CurrentValue * NewMaxValue / CurrentMaxValue) - CurrentValue : NewMaxValue;
		AbilitySystemComponent->ApplyModToAttributeUnsafe(HandleAttributeProperty, EGameplayModOp::Additive, NewDelta);
	}
}

void UBlurAttributeSet::SetRageCheck(const struct FGameplayEffectModCallbackData& Data, float NewCurrentVal)
{
	NewCurrentVal = FMath::Clamp(NewCurrentVal, 0.f, GetMaxRage());
	SetRage(NewCurrentVal);

	//愤怒值达到最大。
	if (GetRage() >= GetMaxRage())
	{
		UBlurFunctionLibrary::AddGameplayTagToActorIfNone(Data.Target.GetAvatarActor(), BlurGameplayTags::Common_Status_Rage_Full);
	}
	//愤怒值变为空。
	else if (GetRage() <= 0.f)
	{
		UBlurFunctionLibrary::AddGameplayTagToActorIfNone(Data.Target.GetAvatarActor(), BlurGameplayTags::Common_Status_Rage_Empty);
	}
	//移除愤怒值最大和愤怒值空Tag。
	else
	{
		UBlurFunctionLibrary::RemoveGameplayTagFromActorIfFound(Data.Target.GetAvatarActor(),BlurGameplayTags::Common_Status_Rage_Full);
		UBlurFunctionLibrary::RemoveGameplayTagFromActorIfFound(Data.Target.GetAvatarActor(),BlurGameplayTags::Common_Status_Rage_Empty);
	}

	//UI更新广播
	if(const UBlurCharacterUIComponent* CharacterUIComponent = CachedPawnUIInterface->GetCharacterUIComponent())
	{
		CharacterUIComponent->OnRageChanged.Broadcast(GetRage() / GetMaxRage());
	}
}
