// Blur Feng All Rights Reserved.


#include "GameFramework/Animation/BlurLinkedAnimLayer.h"

#include "GameFramework/Animation/BlurCharacterAnimInstance.h"

UBlurCharacterAnimInstance* UBlurLinkedAnimLayer::GetAnimInstance() const
{
	return Cast<UBlurCharacterAnimInstance>(GetOwningComponent()->GetAnimInstance());
}
