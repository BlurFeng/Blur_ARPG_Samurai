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

	if (GetCharacterUIComponentFromActorInfo())
		GetCharacterUIComponentFromActorInfo()->OnCheckCooldown.Broadcast(bAllow, AbilityTag);
}

ABlurPlayableCharacter* UBlurPlayableCharacterGameplayAbility::GetPlayableCharacterFromActorInfo()
{
	if (!CurrentActorInfo) return nullptr;
	
	if(!CachedWarriorHeroCharacter.IsValid())
	{
		CachedWarriorHeroCharacter = Cast<ABlurPlayableCharacter>(CurrentActorInfo->AvatarActor);
	}
	
	return CachedWarriorHeroCharacter.IsValid() ? CachedWarriorHeroCharacter.Get() : nullptr;
}

ABlurPlayerController* UBlurPlayableCharacterGameplayAbility::GetPlayerControllerFromActorInfo()
{
	if(!CachedWarriorHeroController.IsValid())
	{
		CachedWarriorHeroController = Cast<ABlurPlayerController>(CurrentActorInfo->PlayerController);
	}

	return CachedWarriorHeroController.IsValid() ? CachedWarriorHeroController.Get() : nullptr;
}
