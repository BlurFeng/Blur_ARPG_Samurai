// Blur Feng All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BlurAnimInstanceBase.generated.h"

struct FGameplayTag;
//动画实例基类。
UCLASS()
class BLURARPGFRAMEWORK_API UBlurAnimInstanceBase : public UAnimInstance
{
	GENERATED_BODY()

protected:
	/// 确认所有者拥有某个Tag。
	/// @param Tag 
	/// @return 
	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe))
	bool OwnerHaveTag(const FGameplayTag Tag) const;
};
