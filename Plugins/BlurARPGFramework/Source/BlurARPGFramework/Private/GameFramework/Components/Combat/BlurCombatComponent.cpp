// Blur Feng All Rights Reserved.


#include "GameFramework/Components/Combat/BlurCombatComponent.h"

#include "Components/BoxComponent.h"
#include "GameFramework/Common/BlurDebugHelper.h"
#include "GameFramework/Items/Weapons/BlurAbilityWeapon.h"
#include "GameFramework/Items/Weapons/BlurWeapon.h"

bool UBlurCombatComponent::GetWeapon(const FGameplayTag InWeaponTag, ABlurWeapon* InWeapon, const bool bRegisterAsEquippedWeapon)
{
	if (!RegisterWeapon(InWeaponTag, InWeapon))
	{
		Debug::Print(FString::Printf(TEXT("A named %s has already been added as carried weapon."), *InWeaponTag.ToString()));
		return false;
	}

	// 当生成武器时，如果武器直接进行装备时，直接设置CurrentEquippedWeaponTag。
	// 否则一般通过“装备武器”技能，并在Ability蓝图中修改。
	if(bRegisterAsEquippedWeapon)
	{
		Equip(InWeaponTag);
	}

	return true;
}

bool UBlurCombatComponent::DiscardWeapon(const FGameplayTag InWeaponTag)
{
	if (!IsCarriedWeapon(InWeaponTag))
	{
		Debug::Print(FString::Printf(TEXT("A named %s has not find in carried weapon."), *InWeaponTag.ToString()));
		return false;
	}

	// 如果是装备中的装备，先卸下装备。
	if (IsCurrentEquippedWeapon(InWeaponTag))
	{
		Unequip(InWeaponTag);
	}

	// 反注册武器。
	UnregisterWeapon(InWeaponTag);
	
	return true;
}

bool UBlurCombatComponent::Equip(const FGameplayTag InWeaponTag)
{
	if (!IsCarriedWeapon(InWeaponTag)) return false;
	if (CurrentEquippedWeaponTag == InWeaponTag) return false;

	// 装备其他武器时，先卸下当前的。
	if (CurrentEquippedWeaponTag != FGameplayTag::EmptyTag)
	{
		if (!Unequip(CurrentEquippedWeaponTag))
			return false;
	}
	
	ABlurWeapon* Weapon = CarriedWeaponsMap.FindRef(InWeaponTag);
	
	// TODO：装备武器的流程。
	if (ABlurAbilityWeapon* AbilityWeapon = Cast<ABlurAbilityWeapon>(Weapon))
	{
		AbilityWeapon->HandleEquipWeapon(GetOwner());
	}
	CurrentEquippedWeaponTag = InWeaponTag;

	return true;
}

bool UBlurCombatComponent::Unequip(const FGameplayTag InWeaponTag)
{
	if (!IsCarriedWeapon(InWeaponTag)) return false;
	if (CurrentEquippedWeaponTag != InWeaponTag) return false;

	ABlurWeapon* Weapon = CarriedWeaponsMap.FindRef(InWeaponTag);
	
	// TODO：卸下武器的流程。
	if (ABlurAbilityWeapon* AbilityWeapon = Cast<ABlurAbilityWeapon>(Weapon))
	{
		AbilityWeapon->HandleUnequipWeapon(GetOwner());
	}
	
	CurrentEquippedWeaponTag = FGameplayTag::EmptyTag;
	return true;
}

bool UBlurCombatComponent::UnequipCurrent()
{
	if (CurrentEquippedWeaponTag == FGameplayTag::EmptyTag)
		return false;
	return Unequip(CurrentEquippedWeaponTag);
}

bool UBlurCombatComponent::RegisterWeapon(FGameplayTag InWeaponTag, ABlurWeapon* InWeapon)
{
	if (CarriedWeaponsMap.Contains(InWeaponTag)) return false;
	check(InWeapon);

	CarriedWeaponsMap.Emplace(InWeaponTag, InWeapon);

	// 绑定回调方法到武器委托。
	InWeapon->OnWeaponMeleeHitTarget.BindUObject(this, &UBlurCombatComponent::OnMeleeHitTargetActor);
	InWeapon->OnWeaponMeleePulledFromTarget.BindUObject(this, &UBlurCombatComponent::OnMeleePulledFromTargetActor);
	
	return true;
}

bool UBlurCombatComponent::UnregisterWeapon(const FGameplayTag InWeaponTag)
{
	if (!CarriedWeaponsMap.Contains(InWeaponTag)) return false;

	ABlurWeapon* Weapon = CarriedWeaponsMap.FindRef(InWeaponTag);

	// 解绑委托。
	Weapon->OnWeaponMeleeHitTarget.Unbind();
	Weapon->OnWeaponMeleePulledFromTarget.Unbind();
	
	CarriedWeaponsMap.Remove(InWeaponTag);
	return true;
}

ABlurWeapon* UBlurCombatComponent::GetCarriedWeaponByTag(const FGameplayTag InWeaponTagToGet) const
{
	// Notes：双重指针。
	// TMap存储的实际上是指向常量指针的指针，我们通过解指针获得需要的指向常量的指针并作为返回值。
	// const*的作用是确保通过双重指针无法直接修改最终指向的常量。
	// AActor* 普通指针； AActor* const 常量指针，指向地址不可修改； const AActor* 只读指针，内容不可修改；
	// const AActor* const 完全常量指针，地址和内容都不可更改； AActor* const* 指向常量指针的指针；
	// const AActor* const* 指向 指向只读内容的常量指针 的指针 

	if(CarriedWeaponsMap.Contains(InWeaponTagToGet))
	{
		if(ABlurWeapon* const* FoundWeapon = CarriedWeaponsMap.Find(InWeaponTagToGet))
		{
			return *FoundWeapon;
		}
	}

	return nullptr;
}

bool UBlurCombatComponent::IsCarriedWeapon(const FGameplayTag InWeaponTag) const
{
	return CarriedWeaponsMap.Contains(InWeaponTag);
}

ABlurWeapon* UBlurCombatComponent::GetCurrentEquippedWeapon() const
{
	if(!CurrentEquippedWeaponTag.IsValid()) return nullptr;
	
	return GetCarriedWeaponByTag(CurrentEquippedWeaponTag);
}

bool UBlurCombatComponent::IsCurrentEquippedWeapon(const FGameplayTag InWeaponTag) const
{
	return CurrentEquippedWeaponTag == InWeaponTag;
}

void UBlurCombatComponent::ToggleWeaponCollision(const bool bShouldEnable, const EToggleDamageType ToggleDamageType)
{
	if(ToggleDamageType == EToggleDamageType::CurrentEquippedWeapon)
	{
		ToggleCurrentEquippedWeaponCollision(bShouldEnable);
	}
	else
	{
		ToggleBodyCollisionBoxCollision(bShouldEnable, ToggleDamageType);
	}
}

void UBlurCombatComponent::OnMeleeHitTargetActor(AActor* HitActor)
{
}

void UBlurCombatComponent::OnMeleePulledFromTargetActor(AActor* HitActor)
{
}

void UBlurCombatComponent::ToggleCurrentEquippedWeaponCollision(const bool bShouldEnable)
{
	const ABlurWeapon* WeaponToToggle = GetCurrentEquippedWeapon();

	check(WeaponToToggle);

	// 开启或关闭武器碰撞盒检测。
	if (bShouldEnable)
	{
		// Debug::Print(WeaponToToggle->GetName() + TEXT("Collision enabled."), FColor::Green);
		// QueryOnly 仅查询。能够检测到碰撞但碰撞盒没有实际的物理体积。
		WeaponToToggle->GetWeaponCollisionBox()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}
	else
	{
		// Debug::Print(WeaponToToggle->GetName() + TEXT("Collision disabled."), FColor::Red);
		WeaponToToggle->GetWeaponCollisionBox()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		// 在关闭碰撞盒时，清空造成过效果的目标Actors缓存列表。
		OverlappedActors.Empty();
	}
}

void UBlurCombatComponent::ToggleBodyCollisionBoxCollision(const bool bShouldEnable,
	const EToggleDamageType ToggleDamageType)
{
}
