// Blur Feng All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/BlurActorBase.h"
#include "GameFramework/Types/BlurStructTypes.h"
#include "GameFramework/Items/Weapons/BlurWeaponObject.h"

#include "BlurWeapon.generated.h"

class UBoxComponent;



// 武器基类。武器能够赋予角色新的技能。
UCLASS()
class BLURARPGFRAMEWORK_API ABlurWeapon : public ABlurActorBase
{
	GENERATED_BODY()

public:
	ABlurWeapon();

	virtual void BeginDestroy() override;

	USceneComponent* RootComponent;

	// 武器数据。
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Blur ARPG Framework | Weapon Data")
	FWeaponInfo WeaponData;
	
	FOnTargetInteractedDelegate OnWeaponMeleeHitTarget;
	FOnTargetInteractedDelegate OnWeaponMeleePulledFromTarget;

	// 装备武器到所有者。
	virtual void Equip(AActor* SelfOwner);

	// 卸下武器从所有者。
	virtual void Unequip();

	// 进入战斗状态。拔出武器。
	virtual void EnterCombat();

	// 离开战斗状态。收起武器。
	virtual void ExitCombat();

	// 获取拥有者角色。
	UFUNCTION(BlueprintPure, Category = "Blur ARPG Framework | Weapon")
	bool GetOwnerCharacter(ABlurCharacterBase*& OwnerCharacter) const;

	/// 设置武器碰撞
	/// @param CollisionEnabled 
	void SetCollisionEnabled(const ECollisionEnabled::Type CollisionEnabled);
	
protected:
	// 拥有者角色。
	TWeakObjectPtr<ABlurCharacterBase> OwnerCharacterWeakPtr;

	// 武器拥有的物体Actor，比如刀和刀鞘。
	TArray<ABlurWeaponObject*> WeaponObjects;
};