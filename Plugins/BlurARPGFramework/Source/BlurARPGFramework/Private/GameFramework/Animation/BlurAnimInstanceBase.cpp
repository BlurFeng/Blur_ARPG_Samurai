// Blur Feng All Rights Reserved.


#include "GameFramework/Animation/BlurAnimInstanceBase.h"

#include "GameFramework/BlurFunctionLibrary.h"

bool UBlurAnimInstanceBase::OwnerHaveTag(const FGameplayTag Tag) const
{
	if (APawn* OwningPawn = TryGetPawnOwner())
	{
		return UBlurFunctionLibrary::ActorHasMatchingGameplayTag(OwningPawn, Tag);
	}

	return false;
}
