// Blur Feng All Rights Reserved.


#include "GameFramework/GameplayAbilitySystem/Abilities/BlurGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameFramework/BlurFunctionLibrary.h"
#include "GameFramework/Characters/BlurCharacterBase.h"
#include "GameFramework/Components/Combat/BlurAbilityCombatComponent.h"
#include "GameFramework/GameplayAbilitySystem/BlurAbilitySystemComponent.h"

UBlurGameplayAbility::UBlurGameplayAbility()
{
	OnCheckCostDelegate.AddUniqueDynamic(this, &ThisClass::OnCheckCost);
	OnCheckCooldownDelegate.AddUniqueDynamic(this, &ThisClass::OnCheckCooldown);
}

void UBlurGameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	// 技能激活策略为给定形式
	if(AbilityActivationPolicy == EAbilityActivationPolicy::OnGiven)
	{
		if(ActorInfo && !Spec.IsActive())
		{
			// 尝试激活技能
			ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle);
		}
	}
}

void UBlurGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	// 技能激活策略为给定形式
	if(AbilityActivationPolicy == EAbilityActivationPolicy::OnGiven)
	{
		if(ActorInfo)
		{
			// 立即结束清除此技能
			ActorInfo->AbilitySystemComponent->ClearAbility(Handle);
		}
	}
}

bool UBlurGameplayAbility::CheckCost(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const
{
	const bool bAllow = Super::CheckCost(Handle, ActorInfo, OptionalRelevantTags);

	OnCheckCostDelegate.Broadcast(bAllow, GetAssetTags().First());
	
	return bAllow;
}

bool UBlurGameplayAbility::CheckCooldown(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const
{
	const bool bAllow = Super::CheckCooldown(Handle, ActorInfo, OptionalRelevantTags);

	OnCheckCooldownDelegate.Broadcast(bAllow, GetAssetTags().First());

	return bAllow;
}

void UBlurGameplayAbility::OnCheckCost(const bool bAllow, const FGameplayTag AbilityTag)
{
}

void UBlurGameplayAbility::OnCheckCooldown(const bool bAllow, const FGameplayTag AbilityTag)
{
}

bool UBlurGameplayAbility::CheckConditionOnToggleableCancelAbility_Implementation()
{
	return true;
}

ABlurCharacterBase* UBlurGameplayAbility::GetCharacterFromActorInfo()
{
	if (!CurrentActorInfo) return nullptr;
	
	if(!CachedBlurCharacterBase.IsValid())
	{
		CachedBlurCharacterBase = Cast<ABlurCharacterBase>(CurrentActorInfo->AvatarActor);
	}
	
	return CachedBlurCharacterBase.IsValid() ? CachedBlurCharacterBase.Get() : nullptr;
}

UBlurAbilityCombatComponent* UBlurGameplayAbility::GetCombatComponentFromActorInfo() const
{
	if (!CurrentActorInfo) return nullptr;
	
	// FindComponentByClass通过遍历查找返回第一个有效的目标。当存在多个UPawnCombatComponent的子类时可能无法按预期工作。
	return GetAvatarActorFromActorInfo()->FindComponentByClass<UBlurAbilityCombatComponent>();
}

UBlurPawnUIComponent* UBlurGameplayAbility::GetPawnUIComponentFromActorInfo()
{
	if (!CurrentActorInfo) return nullptr;
	
	return GetCharacterFromActorInfo()->GetPawnUIComponent();
}

UBlurCharacterUIComponent* UBlurGameplayAbility::GetCharacterUIComponentFromActorInfo()
{
	if (!CurrentActorInfo) return nullptr;
	
	return GetCharacterFromActorInfo()->GetCharacterUIComponent();
}

UBlurAbilitySystemComponent* UBlurGameplayAbility::GetBlurAbilitySystemComponentFromActorInfo() const
{
	return Cast<UBlurAbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent);
}

FActiveGameplayEffectHandle UBlurGameplayAbility::NativeApplyEffectSpecHandleTarget(AActor* TargetActor,
	const FGameplayEffectSpecHandle& InSpecHandle) const
{
	if (!CurrentActorInfo) return FActiveGameplayEffectHandle();
	
	UAbilitySystemComponent* TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	return GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(
		*InSpecHandle.Data,
		TargetAbilitySystemComponent
		);
}

// Tips：这里不适用const方法，是应为const后方法在蓝图中是非执行节点，没有Exec节点可以连接。
FActiveGameplayEffectHandle UBlurGameplayAbility::BP_ApplyEffectSpecHandleTarget(AActor* TargetActor,
	const FGameplayEffectSpecHandle& InSpecHandle, ESuccessType& OutSuccessType)
{
	const FActiveGameplayEffectHandle ActiveGameplayEffectHandle = NativeApplyEffectSpecHandleTarget(TargetActor, InSpecHandle);

	OutSuccessType = ActiveGameplayEffectHandle.WasSuccessfullyApplied() ? ESuccessType::Successful : ESuccessType::Failed;

	return ActiveGameplayEffectHandle;
}

// Tips：这里不为方法添加 const，否则此方法在蓝图中为成为 Pure 相同的节点，没有执行引脚。
void UBlurGameplayAbility::ApplyGameplayEffectSpecHandleToHitResults(const FGameplayEffectSpecHandle& InSpecHandle,
	const int32 TargetFlags, const FGameplayTag HitReactEventTag, const TArray<FHitResult>& InHitResults)
{
	if (InHitResults.IsEmpty()) return;
	
	const APawn* OwningPawn = Cast<APawn>(GetAvatarActorFromActorInfo());
	if (!OwningPawn) return;
	
	for (const FHitResult& HitResult : InHitResults)
	{
		if (APawn* HitPawn = Cast<APawn>(HitResult.GetActor()))
		{
			if (TargetFlags == 0 || TargetFlags & 1 << static_cast<int32>(UBlurFunctionLibrary::CheckTeamAttitude(OwningPawn, HitPawn)))
			{
				// 对目标造成伤害影响。
				FActiveGameplayEffectHandle ActiveGameplayEffectHandle = NativeApplyEffectSpecHandleTarget(HitPawn, InSpecHandle);
				
				if (ActiveGameplayEffectHandle.WasSuccessfullyApplied())
				{
					FGameplayEventData Data;
					Data.Instigator = OwningPawn;
					Data.Target = HitPawn;

					// 目标被击中事件。
					UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
						HitPawn,
						HitReactEventTag,
						Data);
				}
			}
		}
	}
}

FGameplayEffectSpecHandle UBlurGameplayAbility::MakeSpecHandle(const TSubclassOf<UGameplayEffect> EffectClass) const
{
	// 创建EffectContext。这是上下文信息，帮助技能系统获取执行技能所需的信息。
	// 比如在使用UGameplayEffectExecutionCalculation类计算伤害时，通过Context我们能获取我们需要的内容。
	// 搜索 UGEExecCalc_DamageTaken::Execute_Implementation 来查看其中一个计算用类。
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponentFromActorInfo()->MakeEffectContext();
	ContextHandle.SetAbility(this);
	ContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());
	ContextHandle.AddInstigator(GetAvatarActorFromActorInfo(), GetAvatarActorFromActorInfo());
	
	// 创建FGameplayEffectSpecHandle
	FGameplayEffectSpecHandle EffectSpecHandle = GetAbilitySystemComponentFromActorInfo()->MakeOutgoingSpec(
		EffectClass,
		GetAbilityLevel(),
		ContextHandle
		);

	return EffectSpecHandle;
}

// Notes：蓝图方法的参数和 & 关键字。
// 在蓝图中，像 TSubclassOf<UGameplayEffect> 这样的参数，如果想使用UE的资源映射直接选择一个参数，不应当使用 & 关键字。
// 否则你必须创建一个成员字段然后传给 TSubclassOf<UGameplayEffect> 参数。

FGameplayEffectSpecHandle UBlurGameplayAbility::MakeSpecHandleSetByCallerMagnitude(
	const TSubclassOf<UGameplayEffect> EffectClass, const FGameplayTag GameplayTag, const float Magnitude) const
{
	// 创建GE查询句柄。
	FGameplayEffectSpecHandle EffectSpecHandle = MakeSpecHandle(EffectClass);
	EffectSpecHandle.Data->SetSetByCallerMagnitude(GameplayTag, Magnitude);

	return EffectSpecHandle;
}

FGameplayEffectSpecHandle UBlurGameplayAbility::MakeDamageEffectSpecHandle(
	const TSubclassOf<UGameplayEffect> EffectClass, const float InBaseDamage,
	const float InBaseDamageMultiplyCoefficient, const int32 DamageIncreaseCount,
	const float DamageIncreaseCoefficient) const
{
	check(EffectClass);

	// 创建GE查询句柄。
	FGameplayEffectSpecHandle EffectSpecHandle = MakeSpecHandle(EffectClass);

	// Notes：我们可以将数据缓存到Data并在之后使用，存储的数据通过Tag查询。

	// Tips：SetByCallerTagMagnitudes 缓存到Data中的值用于计算。搜索 SetByCallerTagMagnitudes 找到使用处。
	// 缓存基础伤害到Data。用于之后的计算。
	EffectSpecHandle.Data->SetSetByCallerMagnitude(BlurGameplayTags::GEExecCalc_SetByCaller_Attack_BaseDamage, InBaseDamage);
	EffectSpecHandle.Data->SetSetByCallerMagnitude(BlurGameplayTags::GEExecCalc_SetByCaller_Attack_BaseDamageMultiplyCoefficient, InBaseDamageMultiplyCoefficient);

	// 缓存增伤计数和系数到Data。用于之后的计算。
	EffectSpecHandle.Data->SetSetByCallerMagnitude(BlurGameplayTags::GEExecCalc_SetByCaller_Attack_DamageIncreaseCount, DamageIncreaseCount);
	EffectSpecHandle.Data->SetSetByCallerMagnitude(BlurGameplayTags::GEExecCalc_SetByCaller_Attack_DamageIncreaseCoefficient, DamageIncreaseCoefficient);
	
	return EffectSpecHandle;
}

FGameplayEffectSpecHandle UBlurGameplayAbility::MakeDamageEffectSpecHandleByScalableFloat(
	const TSubclassOf<UGameplayEffect> EffectClass, const FScalableFloat& InBaseDamageScalableFloat,
	const float InBaseDamageMultiplyCoefficient, const int32 DamageIncreaseCount,
	const float DamageIncreaseCoefficient) const
{
	return MakeDamageEffectSpecHandle(
		EffectClass, GetScalableFloatValueAtLevel(InBaseDamageScalableFloat), InBaseDamageMultiplyCoefficient, DamageIncreaseCount, DamageIncreaseCoefficient);
}

float UBlurGameplayAbility::GetScalableFloatValueAtLevel(const FScalableFloat& InScalableFloat) const
{
	return UBlurFunctionLibrary::GetScalableFloatValueAtLevel(InScalableFloat, GetAbilityLevel());
}

bool UBlurGameplayAbility::GetAbilityRemainingCooldownByTag(const FGameplayTag InCooldownTag, float& TotalCooldownTime,
	float& RemainingCooldownTime) const
{
	TotalCooldownTime = 0.f;
	RemainingCooldownTime = 0.f;
	
	check(InCooldownTag.IsValid());

	const FGameplayEffectQuery CooldownQuery = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(InCooldownTag.GetSingleTagContainer());
	
	const TArray<TPair<float, float>> TimeRemainingAndDuration = GetAbilitySystemComponentFromActorInfo()->GetActiveEffectsTimeRemainingAndDuration(CooldownQuery);

	if (!TimeRemainingAndDuration.IsEmpty())
	{
		RemainingCooldownTime = TimeRemainingAndDuration[0].Key;
		TotalCooldownTime = TimeRemainingAndDuration[0].Value;
	}

	return RemainingCooldownTime > 0.f;
}
