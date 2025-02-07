// Blur Feng All Rights Reserved.


#include "GameFramework/GameplayAbilitySystem/Abilities/BlurPlayableCharacterGameplayAbility.h"

#include "AbilitySystemComponent.h"
#include "GameFramework/Characters/BlurPlayableCharacter.h"
#include "GameFramework/Components/UI/BlurCharacterUIComponent.h"
#include "GameFramework/Controllers/BlurPlayerController.h"

void UBlurPlayableCharacterGameplayAbility::OnCheckCost(const bool bAllow, const FGameplayTag AbilityTag)
{
	Super::OnCheckCost(bAllow, AbilityTag);

	if (GetCharacterUIComponentFromActorInfo())
		GetCharacterUIComponentFromActorInfo()->OnCheckCost.Broadcast(bAllow, AbilityTag);
}

void UBlurPlayableCharacterGameplayAbility::OnCheckCooldown(const bool bAllow, const FGameplayTag AbilityTag)
{
	Super::OnCheckCooldown(bAllow, AbilityTag);

	// Tips: 在技能通过GiveAbility方法被赋予给技能组件时，此处会被调用到一次。
	// 然而 CurrentActorInfo 实际上并未准备好，为null。

	if (GetCharacterUIComponentFromActorInfo())
		GetCharacterUIComponentFromActorInfo()->OnCheckCooldown.Broadcast(bAllow, AbilityTag);
}

ABlurPlayableCharacter* UBlurPlayableCharacterGameplayAbility::GetPlayableCharacterFromActorInfo()
{
	if (!CurrentActorInfo) return nullptr;
	
	if(!CachedBlurPlayableCharacter.IsValid())
	{
		CachedBlurPlayableCharacter = Cast<ABlurPlayableCharacter>(CurrentActorInfo->AvatarActor);
	}
	
	return CachedBlurPlayableCharacter.IsValid() ? CachedBlurPlayableCharacter.Get() : nullptr;
}

ABlurPlayerController* UBlurPlayableCharacterGameplayAbility::GetPlayerControllerFromActorInfo()
{
	if(!CachedBlurPlayerController.IsValid())
	{
		CachedBlurPlayerController = Cast<ABlurPlayerController>(CurrentActorInfo->PlayerController);
	}

	return CachedBlurPlayerController.IsValid() ? CachedBlurPlayerController.Get() : nullptr;
}
