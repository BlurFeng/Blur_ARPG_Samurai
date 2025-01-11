// Blur Feng All Rights Reserved.


#include "GameFramework/Items/Weapons/BlurAbilityWeapon.h"

void ABlurAbilityWeapon::HandleEquipWeapon(UBlurAbilitySystemComponent* AbilitySystemComponent,
                                           USkeletalMeshComponent* SkeletalMeshComponent, APlayerController* PlayerController)
{
	//TODO：代码重构。
}

void ABlurAbilityWeapon::AssignGrantedAbilitySpecHandles(const TArray<FGameplayAbilitySpecHandle>& InSpecHandles)
{
	GaveAbilitySpecHandles = InSpecHandles;
}

TArray<FGameplayAbilitySpecHandle> ABlurAbilityWeapon::GetGaveAbilitySpecHandles() const
{
	return GaveAbilitySpecHandles;
}
