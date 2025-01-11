// Blur Feng All Rights Reserved.


#include "GameFramework/Types/BlurStructTypes.h"

bool FGiveAbilitySet::IsValid() const
{
	return InputTag.IsValid() && AbilityToGive;
}

#if WITH_EDITOR
void FGiveAbilitySet::PostEditChangeProperty()
{
	//成员属性变化时被调用，可在此处按需求调整或限制字段。
}
#endif