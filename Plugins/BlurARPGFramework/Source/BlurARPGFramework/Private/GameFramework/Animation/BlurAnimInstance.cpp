// Blur Feng All Rights Reserved.


#include "GameFramework/Animation/BlurAnimInstance.h"

#include "GameFramework/BlurFunctionLibrary.h"

bool UBlurAnimInstance::OwnerHaveTag(const FGameplayTag Tag) const
{
	if (APawn* OwningPawn = TryGetPawnOwner())
	{
		return UBlurFunctionLibrary::ActorHasMatchingGameplayTag(OwningPawn, Tag);
	}

	return false;
}
