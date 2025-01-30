// Blur Feng All Rights Reserved.


#include "GameFramework/GameplayAbilitySystem/BlurAbilitySystemComponent.h"

#include "GameFramework/BlurFunctionLibrary.h"
#include "GameFramework/Components/UI/BlurCharacterUIComponent.h"
#include "GameFramework/GameplayAbilitySystem/Abilities/BlurGameplayAbility.h"
#include "GameFramework/Interfaces/BlurPawnUIInterface.h"
#include "GameFramework/Types/BlurEnumTypes.h"
#include "GameFramework/Types/BlurStructTypes.h"

UBlurAbilitySystemComponent::UBlurAbilitySystemComponent()
{
	
}

void UBlurAbilitySystemComponent::OnAbilityInputPressed(const FGameplayTag& InInputTag)
{
	if (!InInputTag.IsValid()) return;

	// Debug::Print(TEXT("OnAbilityInputPressed 1"));

	EBlurInputType InputType = EBlurInputType::Normal;
	if (InInputTag.MatchesTag(BlurGameplayTags::Input_Toggleable))
		InputType = EBlurInputType::Toggleable;
	else if (InInputTag.MatchesTag(BlurGameplayTags::Input_MustBeHeld))
		InputType = EBlurInputType::MustBeHeld;

	for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
	{
		// Tips：InInputTag 通过 UBlurFunctionLibrary::GetGameplayAbilitySpec 方法在赋予技能时添加到 DynamicAbilityTags。
		
		// 确认输入的技能是否存在，通过对比InInputTag。此Tag应当在启动时被添加。
		if (!AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InInputTag)) continue;

		if (InputType == EBlurInputType::Normal)
		{
			// 尝试触发技能。
			TryActivateAbility(AbilitySpec.Handle);
		}
		// 切换形式的技能。在激活和取消之间切换。比如可切换的愤怒状态。
		else if (InputType == EBlurInputType::Toggleable)
		{
			if (!AbilitySpec.IsActive())
			{
				// 尝试触发技能。
				TryActivateAbility(AbilitySpec.Handle);
			}
			else
			{
				bool Allow = false;
				// 切换形式的技能如果想在取消时进行确认，可重写 CheckCondition() 方法。不重写时默认返回true。
				if (UBlurGameplayAbility* GameplayAbility = Cast<UBlurGameplayAbility>(AbilitySpec.GetPrimaryInstance()))
				{
					if (GameplayAbility->CheckConditionOnToggleableCancelAbility())
					{
						CancelAbilityHandle(AbilitySpec.Handle);
						Allow = true;
					}
					// Debug::Print(FString::Printf(TEXT("Cancel Ability, Name: %s"), *AbilitySpec.Ability.GetName()));
				}
				else
				{
					CancelAbilityHandle(AbilitySpec.Handle);
					Allow = true;
				}

				if (const IBlurPawnUIInterface* PawnUIInterface = Cast<IBlurPawnUIInterface>(GetAvatarActor()))
				{
					if (PawnUIInterface->GetCharacterUIComponent())
						PawnUIInterface->GetCharacterUIComponent()->OnCancelAbility.Broadcast(Allow, AbilitySpec.GetPrimaryInstance()->GetAssetTags().First());
				}
			}
		}
		// 必须持续按住的技能，松开时取消。比如防御。
		else if (InputType == EBlurInputType::MustBeHeld)
		{
			CachedMustBeHeldGameplayAbilityInputTag = InInputTag;
			CachedMustBeHeldFGameplayAbilitySpecHandle = AbilitySpec.Handle;

			// 尝试触发技能。
			TryActivateAbility(AbilitySpec.Handle);
		}

		break;
	}
}

void UBlurAbilitySystemComponent::OnAbilityInputReleased(const FGameplayTag& InInputTag)
{
	if (!InInputTag.IsValid()) return;
	
	// 如果是必须持续按住的技能，在松开时取消技能。
	if (InInputTag.MatchesTag(BlurGameplayTags::Input_MustBeHeld))
	{
		for (const FGameplayAbilitySpec& AbilitySpec : GetActivatableAbilities())
		{
			// 取消正在激活中的技能。
			if (AbilitySpec.GetDynamicSpecSourceTags().HasTagExact(InInputTag) && AbilitySpec.IsActive())
			{
				// 清空缓存的需要持续按住触发的技能Spec。
				CachedMustBeHeldGameplayAbilityInputTag = FGameplayTag::EmptyTag;
				CancelAbilityHandle(AbilitySpec.Handle);

				// Debug::Print(FString::Printf(TEXT("Cancel Ability, Name: %s"), *AbilitySpec.Ability.GetName()));
				break;
			}
		}
	}
}

void UBlurAbilitySystemComponent::OnAbilityInputTriggered(const FGameplayTag& InInputTag)
{
	//在持续按住的技能一直按住时，尝试在技能未激活时（被打断）再触发。
	if (CachedMustBeHeldGameplayAbilityInputTag == InInputTag)
	{
		if (const FGameplayAbilitySpec* AbilitySpec = FindAbilitySpecFromHandle(CachedMustBeHeldFGameplayAbilitySpecHandle))
		{
			if (!AbilitySpec->IsActive())
				TryActivateAbility(AbilitySpec->Handle);
		}
	}
}

void UBlurAbilitySystemComponent::GiveWeaponAbilities(const TArray<FGiveAbilitySet> InDefaultWeaponAbilities,
                                                              const TArray<FSpecialGiveAbilitySet> InSpecialWeaponAbilities, int32 ApplyLevel,
                                                              TArray<FGameplayAbilitySpecHandle>& OutGaveAbilitySpecHandles)
{
	if (InDefaultWeaponAbilities.IsEmpty()) return;

	// 默认武器技能。
	for (const FGiveAbilitySet& AbilitySet : InDefaultWeaponAbilities)
	{
		if (!AbilitySet.IsValid()) continue;
		
		FGameplayAbilitySpec AbilitySpec =
			UBlurFunctionLibrary::GetGameplayAbilitySpec(AbilitySet.AbilityToGive, GetAvatarActor(), ApplyLevel, AbilitySet.InputTag);

		// 缓存赋予技能Handle，方便之后用于移除技能等操作。
		OutGaveAbilitySpecHandles.AddUnique(GiveAbility(AbilitySpec));
	}

	// 特殊武器技能。
	for (const FSpecialGiveAbilitySet& AbilitySet : InSpecialWeaponAbilities)
	{
		if (!AbilitySet.IsValid()) continue;
		
		FGameplayAbilitySpec AbilitySpec =
			UBlurFunctionLibrary::GetGameplayAbilitySpec(AbilitySet.AbilityToGive, GetAvatarActor(), ApplyLevel, AbilitySet.InputTag);

		// 缓存赋予技能Handle，方便之后用于移除技能等操作。
		OutGaveAbilitySpecHandles.AddUnique(GiveAbility(AbilitySpec));
	}
}

void UBlurAbilitySystemComponent::RemoveGaveWeaponAbilities(
	TArray<FGameplayAbilitySpecHandle>& InSpecHandlesToRemove)
{
	if (InSpecHandlesToRemove.IsEmpty()) return;

	for (const FGameplayAbilitySpecHandle& SpecHandle : InSpecHandlesToRemove)
	{
		if(!SpecHandle.IsValid()) continue;
		
		ClearAbility(SpecHandle);
	}

	InSpecHandlesToRemove.Empty();
}

bool UBlurAbilitySystemComponent::TryActivateAbilityByTag(const FGameplayTag AbilityTagToActivate)
{
	check(AbilityTagToActivate.IsValid());

	//查询所有符合条件的技能。
	TArray<FGameplayAbilitySpec*> FoundAbilitySpecs;
	GetActivatableGameplayAbilitySpecsByAllMatchingTags(AbilityTagToActivate.GetSingleTagContainer(), FoundAbilitySpecs);

	if (!FoundAbilitySpecs.IsEmpty())
	{
		//随机触发一个技能。
		const int32 RandomAbilityIndex = FMath::RandRange(0, FoundAbilitySpecs.Num() - 1);
		const FGameplayAbilitySpec* AbilitySpecToActivate = FoundAbilitySpecs[RandomAbilityIndex];

		check(AbilitySpecToActivate);

		if (!AbilitySpecToActivate->IsActive())
		{
			return TryActivateAbility(AbilitySpecToActivate->Handle);
		}
	}

	return false;
}
