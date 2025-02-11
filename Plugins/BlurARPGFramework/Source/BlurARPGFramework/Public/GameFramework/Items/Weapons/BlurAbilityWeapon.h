// Blur Feng All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Items/Weapons/BlurWeapon.h"
#include "GameFramework/Types/BlurStructTypes.h"

#include "BlurAbilityWeapon.generated.h"

// Tips：如果你在构建可以使用不同武器的角色，使用此类作为角色武器的基类。那样角色在使用不同武器时就能获得不同的技能。
// 如果你只是想创建简单的小兵，直接用 ABlurWeaponBase 创建蓝图子类作为武器即可。

class UBlurAbilitySystemComponent;
struct FGameplayAbilitySpecHandle;

// 拥有技能的武器，在装备时赋予拥有者技能。
UCLASS()
class BLURARPGFRAMEWORK_API ABlurAbilityWeapon : public ABlurWeapon
{
	GENERATED_BODY()

public:
	// 技能武器数据。
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Blur ARPG Framework | Weapon Data")
	FAbilityWeaponInfo AbilityWeaponData;

	// 武器技能等级，决定了武器赋予拥有者技能等级。
	UPROPERTY(BlueprintReadWrite, Category = "Blur ARPG Framework | Weapon Data")
	int WeaponAbilityLevel;

	virtual void Equip(AActor* SelfOwner) override;
	virtual void Unequip() override;
	virtual void EnterCombat() override;
	virtual void ExitCombat() override;
	
private:
	
	//技能查询句柄。用于记录被赋予拥有者的技能。
	TArray<FGameplayAbilitySpecHandle> GaveAbilitySpecHandlesWithEquip;
	TArray<FGameplayAbilitySpecHandle> GaveAbilitySpecHandlesWithCombat;
};
