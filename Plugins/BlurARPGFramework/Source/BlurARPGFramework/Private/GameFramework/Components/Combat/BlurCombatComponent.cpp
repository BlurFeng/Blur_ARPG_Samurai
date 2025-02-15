// Blur Feng All Rights Reserved.


#include "GameFramework/Components/Combat/BlurCombatComponent.h"

#include "Components/BoxComponent.h"
#include "GameFramework/BlurFunctionLibrary.h"
#include "GameFramework/Common/BlurDebugHelper.h"
#include "GameFramework/Items/Weapons/BlurAbilityWeapon.h"
#include "GameFramework/Items/Weapons/BlurWeapon.h"

bool UBlurCombatComponent::Equip(const FGameplayTag InWeaponTag, ABlurWeapon* InWeapon, const bool bEnterCombatWithWeapon)
{
	if (!InWeapon) return false;

	// 注册武器信息。
	if (!RegisterWeapon(InWeaponTag, InWeapon))
	{
		Debug::Print(FString::Printf(TEXT("A named %s has already been added as carried weapon."), *InWeaponTag.ToString()));
		return false;
	}

	InWeapon->Equip(GetOwner());

	// 直接拔出武器进入战斗状态。
	if(bEnterCombatWithWeapon)
	{
		EnterCombatWithWeapon(InWeaponTag);
	}

	return true;
}

bool UBlurCombatComponent::Unequip(const FGameplayTag InWeaponTag)
{
	ABlurWeapon* Weapon = GetEquippedWeaponByTag(InWeaponTag);
	if (!Weapon)
	{
		Debug::Print(FString::Printf(TEXT("A named %s has not find in carried weapon."), *InWeaponTag.ToString()));
		return false;
	}

	// 如果是战斗状态使用中的武器，先卸下。
	if (IsCurrentCombatWeapon(InWeaponTag))
	{
		ExitCombatWithWeapon(InWeaponTag);
	}

	Weapon->Unequip();

	// 注销武器信息。
	UnregisterWeapon(InWeaponTag);
	
	return true;
}

bool UBlurCombatComponent::EnterCombatWithWeapon(const FGameplayTag InWeaponTag)
{
	if (!IsEquippedWeapon(InWeaponTag)) return false;
	if (CurrentCombatWeapon == InWeaponTag) return false;

	// 先离开战斗状态。放回当前拿着的武器。
	if (CurrentCombatWeapon != FGameplayTag::EmptyTag)
	{
		if (!ExitCombatWithWeapon(CurrentCombatWeapon))
			return false;
	}
	
	ABlurWeapon* Weapon = EquippedWeaponsMap.FindRef(InWeaponTag);
	
	if (ABlurAbilityWeapon* AbilityWeapon = Cast<ABlurAbilityWeapon>(Weapon))
	{
		// 使用此武器进入战斗状态。比如从刀鞘拔出刀。
		AbilityWeapon->EnterCombat();
	}

	// 添加战斗中状态。
	UBlurFunctionLibrary::AddGameplayTagToActorIfNone(GetOwner(), BlurGameplayTags::Common_Status_InCombat);
	
	CurrentCombatWeapon = InWeaponTag;
	return true;
}

bool UBlurCombatComponent::ExitCombatWithWeapon(const FGameplayTag InWeaponTag)
{
	if (!IsEquippedWeapon(InWeaponTag)) return false;
	if (CurrentCombatWeapon != InWeaponTag) return false;

	ABlurWeapon* Weapon = EquippedWeaponsMap.FindRef(InWeaponTag);
	
	if (ABlurAbilityWeapon* AbilityWeapon = Cast<ABlurAbilityWeapon>(Weapon))
	{
		AbilityWeapon->ExitCombat();
	}

	// 移除战斗中状态。
	UBlurFunctionLibrary::RemoveGameplayTagFromActorIfFound(GetOwner(), BlurGameplayTags::Common_Status_InCombat);
	
	CurrentCombatWeapon = FGameplayTag::EmptyTag;
	return true;
}

bool UBlurCombatComponent::ExitCombatWithCurrentCombatWeapon()
{
	if (CurrentCombatWeapon == FGameplayTag::EmptyTag)
		return false;
	return ExitCombatWithWeapon(CurrentCombatWeapon);
}

bool UBlurCombatComponent::RegisterWeapon(FGameplayTag InWeaponTag, ABlurWeapon* InWeapon)
{
	if (EquippedWeaponsMap.Contains(InWeaponTag)) return false;
	check(InWeapon);

	EquippedWeaponsMap.Emplace(InWeaponTag, InWeapon);

	// 绑定回调方法到武器委托。
	InWeapon->OnWeaponMeleeHitTarget.BindUObject(this, &UBlurCombatComponent::OnMeleeHitTargetActor);
	InWeapon->OnWeaponMeleePulledFromTarget.BindUObject(this, &UBlurCombatComponent::OnMeleePulledFromTargetActor);
	
	return true;
}

bool UBlurCombatComponent::UnregisterWeapon(const FGameplayTag InWeaponTag)
{
	if (!EquippedWeaponsMap.Contains(InWeaponTag)) return false;

	ABlurWeapon* Weapon = EquippedWeaponsMap.FindRef(InWeaponTag);

	// 解绑委托。
	Weapon->OnWeaponMeleeHitTarget.Unbind();
	Weapon->OnWeaponMeleePulledFromTarget.Unbind();
	
	EquippedWeaponsMap.Remove(InWeaponTag);
	return true;
}

ABlurWeapon* UBlurCombatComponent::GetEquippedWeaponByTag(const FGameplayTag InWeaponTagToGet) const
{
	// Notes：双重指针。
	// TMap存储的实际上是指向常量指针的指针，我们通过解指针获得需要的指向常量的指针并作为返回值。
	// const*的作用是确保通过双重指针无法直接修改最终指向的常量。
	// AActor* 普通指针； AActor* const 常量指针，指向地址不可修改； const AActor* 只读指针，内容不可修改；
	// const AActor* const 完全常量指针，地址和内容都不可更改； AActor* const* 指向常量指针的指针；
	// const AActor* const* 指向 指向只读内容的常量指针 的指针 

	if(EquippedWeaponsMap.Contains(InWeaponTagToGet))
	{
		if(ABlurWeapon* const* FoundWeapon = EquippedWeaponsMap.Find(InWeaponTagToGet))
		{
			return *FoundWeapon;
		}
	}

	return nullptr;
}

bool UBlurCombatComponent::IsEquippedWeapon(const FGameplayTag InWeaponTag) const
{
	return EquippedWeaponsMap.Contains(InWeaponTag);
}

ABlurWeapon* UBlurCombatComponent::GetCurrentCombatWeapon() const
{
	if(!CurrentCombatWeapon.IsValid()) return nullptr;
	
	return GetEquippedWeaponByTag(CurrentCombatWeapon);
}

bool UBlurCombatComponent::IsCurrentCombatWeapon(const FGameplayTag InWeaponTag) const
{
	return CurrentCombatWeapon == InWeaponTag;
}

void UBlurCombatComponent::ToggleCollisionWithCurrentCombatWeapon(const bool bShouldEnable)
{
	ABlurWeapon* WeaponToToggle = GetCurrentCombatWeapon();

	check(WeaponToToggle);

	// 开启或关闭武器碰撞盒检测。
	if (bShouldEnable)
	{
		// Debug::Print(WeaponToToggle->GetName() + TEXT("Collision enabled."), FColor::Green);
		// QueryOnly 仅查询。能够检测到碰撞但碰撞盒没有实际的物理体积。
		WeaponToToggle->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else
	{
		// Debug::Print(WeaponToToggle->GetName() + TEXT("Collision disabled."), FColor::Red);
		WeaponToToggle->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		// 在关闭碰撞盒时，清空造成过效果的目标Actors缓存列表。
		OverlappedActors.Empty();
	}
}

void UBlurCombatComponent::OnMeleeHitTargetActor(AActor* HitActor)
{
}

void UBlurCombatComponent::OnMeleePulledFromTargetActor(AActor* HitActor)
{
}