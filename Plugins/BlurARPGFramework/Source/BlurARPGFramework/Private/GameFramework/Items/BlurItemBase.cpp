// Blur Feng All Rights Reserved.


#include "GameFramework/Items/BlurItemBase.h"

ABlurItemBase::ABlurItemBase()
{
	
}

void ABlurItemBase::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	TagContainer = GameplayTagContainer;
}

void ABlurItemBase::AddGameplayTag(const FGameplayTag& Tag)
{
	GameplayTagContainer.AddTag(Tag);
}

void ABlurItemBase::RemoveGameplayTag(const FGameplayTag& Tag)
{
	GameplayTagContainer.RemoveTag(Tag);
}
