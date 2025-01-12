// Blur Feng All Rights Reserved.


#include "GameFramework/Components/Combat/BlurAbilityCombatComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GameFramework/BlurFunctionLibrary.h"
#include "GameFramework/Common/WeakObjectPtrExtensions.h"
#include "GameFramework/Items/Weapons/BlurAbilityWeapon.h"
#include "GameFramework/BlurGameplayTags.h"

ABlurAbilityWeapon* UBlurAbilityCombatComponent::GetCarriedAbilityWeaponByTag(FGameplayTag InWeaponTag) const
{
	return Cast<ABlurAbilityWeapon>(GetCarriedWeaponByTag(InWeaponTag));
}

ABlurAbilityWeapon* UBlurAbilityCombatComponent::GetCurrentEquippedAbilityWeapon() const
{
	return Cast<ABlurAbilityWeapon>(GetCurrentEquippedWeapon());
}

float UBlurAbilityCombatComponent::GetCurrentEquippedAbilityWeaponDamageAtLevel(const float InLevel) const
{
	return GetCurrentEquippedAbilityWeapon()->AbilityWeaponData.WeaponBaseDamage.GetValueAtLevel(InLevel);
}

void UBlurAbilityCombatComponent::OnMeleeHitTargetActor(AActor* HitActor)
{
	Super::OnMeleeHitTargetActor(HitActor);

	// Debug::Print(GetOwningPawn()->GetActorNameOrLabel() + TEXT("hit ") + HitActor->GetActorNameOrLabel(), FColor::Green);

	// 确认是否已经对击中目标造成过效果.
	if (WeakObjectPtrExtensions::Contains(OverlappedActors, HitActor)) return;
	WeakObjectPtrExtensions::Add(OverlappedActors, HitActor);

	// 确认是否被格挡。
	bool bIsValidBlock = false;
	const bool bIsPlayerBlocking =
		UBlurFunctionLibrary::ActorHasMatchingGameplayTag(HitActor, BlurGameplayTags::Common_Status_Blocking); // 正在格挡中。
	const bool bIsMyAttackUnBlockable =
		UBlurFunctionLibrary::ActorHasMatchingGameplayTag(GetOwningPawn(), BlurGameplayTags::Common_Status_Unblockable); // 不可阻挡状态。
	if (bIsPlayerBlocking && !bIsMyAttackUnBlockable)
	{
		bIsValidBlock = UBlurFunctionLibrary::IsValidBlock(GetOwningPawn(), HitActor);
	}
	
	// 向自身Pawn发送GameplayEvent事件，告知击中了目标。
	FGameplayEventData Data;
	Data.Instigator = GetOwningPawn();
	Data.Target = HitActor;

	//被击中目标格挡。
	if (bIsValidBlock)
	{
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			HitActor,
			BlurGameplayTags::Common_Event_SuccessfulBlock,
			Data
			);
	}
	//成功击中目标。
	else
	{
		// Tips：在GA中，我们通过 WaitGameplayEvent 方法来等待收到此 Event 后，对目标添加被击中时的GE效果。
		// 给拥有者Pawn发送 MeleeHit 事件。
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
			GetOwningPawn(),
			BlurGameplayTags::Common_Event_MeleeHit,
			Data
			);
	}
}

void UBlurAbilityCombatComponent::OnMeleePulledFromTargetActor(AActor* HitActor)
{
	Super::OnMeleePulledFromTargetActor(HitActor);

	FGameplayEventData Data;
	Data.Instigator = GetOwningPawn();
	Data.Target = HitActor;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		GetOwningPawn(),
		BlurGameplayTags::Common_Event_MeleePulled,
		Data
		);
}

void UBlurAbilityCombatComponent::ToggleBodyCollisionBoxCollision(
	const bool bShouldEnable, const EToggleDamageType ToggleDamageType)
{
	Super::ToggleBodyCollisionBoxCollision(bShouldEnable, ToggleDamageType);
}
