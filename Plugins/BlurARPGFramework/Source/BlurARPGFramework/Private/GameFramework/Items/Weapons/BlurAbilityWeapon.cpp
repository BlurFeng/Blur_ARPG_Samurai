// Blur Feng All Rights Reserved.


#include "GameFramework/Items/Weapons/BlurAbilityWeapon.h"

#include "GameFramework/BlurFunctionLibrary.h"
#include "GameFramework/Characters/BlurCharacterBase.h"
#include "GameFramework/Components/UI/BlurCharacterUIComponent.h"
#include "GameFramework/GameplayAbilitySystem/BlurAbilitySystemComponent.h"

void ABlurAbilityWeapon::Equip(AActor* SelfOwner)
{
	Super::Equip(SelfOwner);

	ABlurCharacterBase* BlurCharacterBase;
	if (!GetOwnerCharacter(BlurCharacterBase)) return;

	// 赋予拥有者装备时技能。
	UBlurAbilitySystemComponent* AbilitySystemComponent = BlurCharacterBase->GetBlurAbilitySystemComponent();
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->GiveWeaponAbilities(
		AbilityWeaponData.AbilitiesWithEquip,
		WeaponAbilityLevel > 0 ? WeaponAbilityLevel : AbilityWeaponData.DefaultWeaponAbilityLevel,
		GaveAbilitySpecHandlesWithEquip);
	}
}

void ABlurAbilityWeapon::Unequip()
{
	Super::Unequip();
	
	ABlurCharacterBase* BlurCharacterBase;
	if (!GetOwnerCharacter(BlurCharacterBase)) return;

	// 清除赋予拥有者的技能。
	if (UBlurAbilitySystemComponent* AbilitySystemComponent = BlurCharacterBase->GetBlurAbilitySystemComponent())
	{
		AbilitySystemComponent->RemoveGaveWeaponAbilities(GaveAbilitySpecHandlesWithEquip);
	}
	
	OwnerCharacterWeakPtr = nullptr;
}

void ABlurAbilityWeapon::EnterCombat()
{
	Super::EnterCombat();
	
	ABlurCharacterBase* BlurCharacterBase;
	if (!GetOwnerCharacter(BlurCharacterBase)) return;
	
	// 赋予拥有者战斗时技能。
	UBlurAbilitySystemComponent* AbilitySystemComponent = BlurCharacterBase->GetBlurAbilitySystemComponent();
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->GiveWeaponAbilities(
		AbilityWeaponData.AbilitiesWithCombat,
		WeaponAbilityLevel > 0 ? WeaponAbilityLevel : AbilityWeaponData.DefaultWeaponAbilityLevel,
		GaveAbilitySpecHandlesWithCombat);

		AbilitySystemComponent->GiveWeaponSpecialAbilities(
		AbilityWeaponData.SpecialAbilitiesWithCombat,
		WeaponAbilityLevel > 0 ? WeaponAbilityLevel : AbilityWeaponData.DefaultWeaponAbilityLevel,
		GaveAbilitySpecHandlesWithCombat);
	}

	// 调用拿出武器时的UI更新委托。
	if (BlurCharacterBase->GetCharacterUIComponent())
	{
		// 更新武器特殊技能UI。
		for (const FGiveSpecialAbilitySet AbilitySet : AbilityWeaponData.SpecialAbilitiesWithCombat)
		{
			// 武器特殊技能Icon更新。
			BlurCharacterBase->GetCharacterUIComponent()->OnAbilityIconSlotUpdatedDelegate.Broadcast(AbilitySet.InputTag, AbilitySet.SoftAbilityIconMaterial);

			float TotalCooldownTime = 0.f;
			float RemainingCooldownTime = 0.f;
			if (UBlurFunctionLibrary::GetAbilityCooldownByTag(AbilitySystemComponent, AbilitySet.AbilityCooldownTag, TotalCooldownTime, RemainingCooldownTime))
			{
				// 武器特殊技能冷却显示更新。
				BlurCharacterBase->GetCharacterUIComponent()->OnAbilityCooldownBegin.Broadcast(AbilitySet.InputTag, TotalCooldownTime, RemainingCooldownTime);
			}
		}
	}
}

void ABlurAbilityWeapon::ExitCombat()
{
	Super::ExitCombat();
	
	ABlurCharacterBase* BlurCharacterBase;
	if (!GetOwnerCharacter(BlurCharacterBase)) return;
	
	// 清除赋予拥有者的技能。
	if (UBlurAbilitySystemComponent* AbilitySystemComponent = BlurCharacterBase->GetBlurAbilitySystemComponent())
	{
		AbilitySystemComponent->RemoveGaveWeaponAbilities(GaveAbilitySpecHandlesWithCombat);
	}

	// 调用放回武器时的UI更新委托。
	if (BlurCharacterBase->GetCharacterUIComponent())
	{
		// 更新武器特殊技能UI。
		for (const FGiveSpecialAbilitySet AbilitySet : AbilityWeaponData.SpecialAbilitiesWithCombat)
		{
			// 武器特殊技能Icon更新为空。
			BlurCharacterBase->GetCharacterUIComponent()->OnAbilityIconSlotUpdatedDelegate.Broadcast(AbilitySet.InputTag, nullptr);

			// 取消UI的冷却计数，防止额外的消耗。
			BlurCharacterBase->GetCharacterUIComponent()->OnAbilityCooldownCancel.Broadcast();
		}
	}
}
