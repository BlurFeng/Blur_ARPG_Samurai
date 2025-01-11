// Blur Feng All Rights Reserved.


#include "GameFramework/Animation/BlurLinkedAnimLayerBase.h"

#include "GameFramework/Animation/BlurCharacterAnimInstanceBase.h"

UBlurCharacterAnimInstanceBase* UBlurLinkedAnimLayerBase::GetHeroAnimInstance() const
{
	return Cast<UBlurCharacterAnimInstanceBase>(GetOwningComponent()->GetAnimInstance());
}
