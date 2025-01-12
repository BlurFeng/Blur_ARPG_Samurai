// Blur Feng All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Animation/BlurAnimInstance.h"
#include "BlurCharacterAnimInstance.generated.h"

class UCharacterMovementComponent;
class ABlurCharacterBase;
// 角色动画实例基类。
UCLASS()
class BLURARPGFRAMEWORK_API UBlurCharacterAnimInstance : public UBlurAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds) override;

protected:

	// Notes：当我们定义一个成员变量时，总是添加 UPROPERTY() ，这样能让UE自动管理这个指针。否则我们需要自己处理内存回收。
	
	UPROPERTY()
	ABlurCharacterBase* OwningCharacter;

	UPROPERTY()
	UCharacterMovementComponent* OwningMovementComponent;

	// 地面速度速速率[0-1]，当前速度达到最大速度时候为1。
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	float GroundSpeedRate;

	// 拥有加速度。
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	bool bHasAcceleration;

	// 移动方向。
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "AnimData|LocomotionData")
	float LocomotionDirection;
};
