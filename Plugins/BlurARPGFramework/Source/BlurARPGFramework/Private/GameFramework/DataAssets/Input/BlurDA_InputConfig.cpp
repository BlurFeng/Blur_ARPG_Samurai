// Blur Feng All Rights Reserved.


#include "GameFramework/DataAssets/Input/BlurDA_InputConfig.h"

UInputAction* UBlurDA_InputConfig::FindNativeInputActionByTag(const FGameplayTag& InInputTag) const
{
	//通过遍历和对比InputTag来查找并返回需要的UInputAction。
	for(const FInputActionConfig& InputActionConfig : NativeInputActions)
	{
		if(InputActionConfig.InputTag == InInputTag && InputActionConfig.InputAction)
		{
			return InputActionConfig.InputAction;
		}
	}

	return nullptr;
}
