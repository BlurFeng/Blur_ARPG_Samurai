// Blur Feng All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/BlurActorBase.h"
#include "BlurWeaponBase.generated.h"

// 武器基类。武器能够赋予角色新的能力。
UCLASS()
class BLURARPGFRAMEWORK_API ABlurWeaponBase : public ABlurActorBase
{
	GENERATED_BODY()

public:
	ABlurWeaponBase();
};
