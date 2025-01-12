// Blur Feng All Rights Reserved.


#include "GameFramework/Items/Weapons/BlurAbilityWeapon.h"

#include "EnhancedInputSubsystems.h"
#include "GameFramework/BlurFunctionLibrary.h"
#include "GameFramework/Animation/BlurLinkedAnimLayer.h"
#include "GameFramework/Characters/BlurCharacterBase.h"
#include "GameFramework/Components/UI/BlurCharacterUIComponent.h"
#include "GameFramework/GameplayAbilitySystem/BlurAbilitySystemComponent.h"

void ABlurAbilityWeapon::HandleEquipWeapon(ABlurCharacterBase* BlurCharacterBase)
{
	// 赋予拥有者武器技能。
	UBlurAbilitySystemComponent* AbilitySystemComponent = BlurCharacterBase->GetBlurAbilitySystemComponent();
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->GiveWeaponAbilities(
		AbilityWeaponData.DefaultWeaponAbilities, AbilityWeaponData.SpecialWeaponAbilities, WeaponAbilityLevel, GaveAbilitySpecHandles);
	}

	// 添加武器的输入映射到本地玩家。
	if (const APlayerController* PlayerController = Cast<APlayerController>(BlurCharacterBase->GetController()))
	{
		if (const ULocalPlayer* LocalPlayer =  PlayerController->GetLocalPlayer())
		{
			UEnhancedInputLocalPlayerSubsystem* Subsystem =
				ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

			// 用优先级更高的武器输入映射覆盖角色的默认输入映射。
			// 多个输入映射可以同时存在，只有发生冲突的输入会优先使用优先级高的。所以在武器输入映射中，没必要重新配置基础移动的部分。
			Subsystem->AddMappingContext(AbilityWeaponData.WeaponInputMappingContext, AbilityWeaponData.WeaponInputMappingPriority);
		}
	}
	
	// 链接动画层到Mesh。根据装备武器不同，我们会有不同的动画表现。
	BlurCharacterBase->GetMesh()->LinkAnimClassLayers(AbilityWeaponData.WeaponAnimLayerToLink);
	
	// 调用装备武器时的UI更新委托。
	if (BlurCharacterBase->GetCharacterUIComponent())
	{
		// 更新武器Icon。
		BlurCharacterBase->GetCharacterUIComponent()->OnEquippedWeaponChangedDelegate.Broadcast(AbilityWeaponData.SoftWeaponIconTexture);

		// 更新武器特殊技能UI。
		for (const FSpecialGiveAbilitySet AbilitySet : AbilityWeaponData.SpecialWeaponAbilities)
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

void ABlurAbilityWeapon::HandleUnequipWeapon(ABlurCharacterBase* BlurCharacterBase)
{
	// 清除赋予拥有者的技能。
	UBlurAbilitySystemComponent* AbilitySystemComponent = BlurCharacterBase->GetBlurAbilitySystemComponent();
	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->RemoveGaveWeaponAbilities(GaveAbilitySpecHandles);
	}

	// 移除武器的输入映射从本地玩家。
	if (const APlayerController* PlayerController = Cast<APlayerController>(BlurCharacterBase->GetController()))
	{
		if (const ULocalPlayer* LocalPlayer =  PlayerController->GetLocalPlayer())
		{
			UEnhancedInputLocalPlayerSubsystem* Subsystem =
				ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

			// 用优先级更高的武器输入映射覆盖角色的默认输入映射。
			// 多个输入映射可以同时存在，只有发生冲突的输入会优先使用优先级高的。所以在武器输入映射中，没必要重新配置基础移动的部分。
			Subsystem->RemoveMappingContext(AbilityWeaponData.WeaponInputMappingContext);
		}
	}

	// 解除链接动画层从Mesh。
	BlurCharacterBase->GetMesh()->UnlinkAnimClassLayers(AbilityWeaponData.WeaponAnimLayerToLink);

	// 调用装备武器时的UI更新委托。
	if (BlurCharacterBase->GetCharacterUIComponent())
	{
		// 更新武器Icon为空。
		BlurCharacterBase->GetCharacterUIComponent()->OnEquippedWeaponChangedDelegate.Broadcast(nullptr);

		// 更新武器特殊技能UI。
		for (const FSpecialGiveAbilitySet AbilitySet : AbilityWeaponData.SpecialWeaponAbilities)
		{
			// 武器特殊技能Icon更新为空。
			BlurCharacterBase->GetCharacterUIComponent()->OnAbilityIconSlotUpdatedDelegate.Broadcast(AbilitySet.InputTag, nullptr);

			// 取消UI的冷却计数，防止额外的消耗。
			BlurCharacterBase->GetCharacterUIComponent()->OnAbilityCooldownCancel.Broadcast();
		}
	}
}
