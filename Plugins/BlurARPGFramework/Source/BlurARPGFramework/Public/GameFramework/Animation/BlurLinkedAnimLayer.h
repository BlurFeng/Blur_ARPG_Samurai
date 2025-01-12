// Blur Feng All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Animation/BlurAnimInstance.h"
#include "BlurLinkedAnimLayer.generated.h"

class UBlurCharacterAnimInstance;
// 链接动画层基类。
UCLASS()
class BLURARPGFRAMEWORK_API UBlurLinkedAnimLayer : public UBlurAnimInstance
{
	GENERATED_BODY()

public:

	// Notes：BlueprintThreadSafe 特性使此方法是线程安全的。
	// UE5新的动画蓝图将逻辑更新从 主线程 转移到了 其他线程。以提高动画蓝图性能。
	// 不是 BlueprintThreadSafe 的方法无法在动画蓝图逻辑流程中使用。
	
	/// 获取动画实例。
	/// @return 
	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe))
	UBlurCharacterAnimInstance* GetAnimInstance() const;
};
