// Blur Feng All Rights Reserved.


#include "GameFramework/Components/Input/BlurEnhancedInputComponent.h"

void UBlurEnhancedInputComponent::RemoveBindings(const TArray<FInputBindingHandle>& InputBindingHandles)
{
	for (const FInputBindingHandle& Handle : InputBindingHandles)
	{
		RemoveBinding(Handle);
	}
}
