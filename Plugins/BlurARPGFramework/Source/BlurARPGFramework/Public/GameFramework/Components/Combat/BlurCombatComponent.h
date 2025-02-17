// Blur Feng All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Components/BlurPawnComponentBase.h"
#include "GameplayTagContainer.h"

#include "BlurCombatComponent.generated.h"

class ABlurWeapon;

// 战斗组件基类。
UCLASS()
class BLURARPGFRAMEWORK_API UBlurCombatComponent : public UBlurPawnComponentBase
{
	GENERATED_BODY()

public:

	// Tips: 组件的使用流程和概念定义。
	// 以一把刀为例。当我们把到从背包移动到角色装备栏时，调用 Equip()。此时武器被实例化并装备到角色腰间。我们实例化“刀”和“刀鞘”两个 WeaponObjet。
	// 当使用刀进入战斗状态时，调用 EnterCombatWithWeapon() 来将武器拿到手上。
	// 注意，播放拔刀动画并将武器的“刀”部件Attach到玩家手上的逻辑需要另外实现，因为根据业务需求不同，这部分内容不尽相同。
	
	// 角色当前战斗状态下拿着的武器。
	UPROPERTY(BlueprintReadOnly, Category = "Blur ARPG Framework | Combat")
	FGameplayTag CurrentCombatWeapon;

	/// 装备武器。比如将刀从背包拿出放在腰间。
	/// @param InWeapon 
	/// @param bEnterCombatWithWeapon 
	/// @return 
	UFUNCTION(BlueprintCallable, Category = "Blur ARPG Framework | Combat")
	bool Equip(ABlurWeapon* InWeapon, const bool bEnterCombatWithWeapon);

	/// 卸下武器。比如将刀从腰间卸下，放回背包。
	/// @param InWeaponTag 
	/// @return 
	UFUNCTION(BlueprintCallable, Category = "Blur ARPG Framework | Combat")
	bool Unequip(const FGameplayTag InWeaponTag);

	/// 进入攻击状态。比如将刀从刀鞘中拔出，只有可以进行攻击。
	/// 通常，你可以先使用Montage播放拔刀动画，根据需求，在动画中或动画结束时调用此方法。
	/// 你可能还需要在动画的某一帧，通过AttachActorToComponent来将武器附加到角色手中。
	/// @param InWeaponTag 
	/// @return 
	UFUNCTION(BlueprintCallable, Category = "Blur ARPG Framework | Combat")
	bool EnterCombatWithWeapon(const FGameplayTag InWeaponTag);

	/// 离开攻击状态。比如将刀放回刀鞘。
	/// 通常，你可以先使用Montage播放拔刀动画，根据需求，在动画中或动画结束时调用此方法。
	/// 你可能还需要在动画的某一帧，通过AttachActorToComponent来将武器附加到刀鞘。
	/// @param InWeaponTag 
	/// @return 
	UFUNCTION(BlueprintCallable, Category = "Blur ARPG Framework | Combat")
	bool ExitCombatWithWeapon(const FGameplayTag InWeaponTag);

	/// 离开攻击状态。收起当前武器。
	/// @return 
	UFUNCTION(BlueprintCallable, Category = "Blur ARPG Framework | Combat")
	bool ExitCombatWithCurrentCombatWeapon();

	/// 注册一个持有武器。当获得武器时调用。
	/// @param InWeaponTag 武器Tag。
	/// @param InWeapon 生成的武器。
	bool RegisterWeapon(const FGameplayTag InWeaponTag, ABlurWeapon* InWeapon);

	bool UnregisterWeapon(const FGameplayTag InWeaponTag);

	/// 获取角色当前装备的武器，通过Tag。注意，这不代表武器正拿着使用中。
	/// @param InWeaponTagToGet 武器Tag。
	/// @return 
	UFUNCTION(BlueprintCallable, Category = "Blur ARPG Framework | Combat")
	ABlurWeapon* GetEquippedWeaponByTag(const FGameplayTag InWeaponTagToGet) const;

	/// 是装备的武器。注意，这不代表武器正拿着使用中。
	/// @param InWeaponTag 
	/// @return 
	UFUNCTION(BlueprintCallable, Category = "Blur ARPG Framework | Combat")
	bool IsEquippedWeapon(const FGameplayTag InWeaponTag) const;

	/// 获取角色当前战斗状态下拿着的武器。
	/// @return 
	UFUNCTION(BlueprintPure, Category = "Blur ARPG Framework | Combat")
	ABlurWeapon* GetCurrentCombatWeapon() const;

	/// 在战斗状态中。正在使用一个武器。
	/// @return 
	UFUNCTION(BlueprintPure, Category = "Blur ARPG Framework | Combat")
	bool IsInCombat() const;

	/// 确认是否是当前战斗状态下持有的武器。
	/// @param InWeaponTag 
	/// @return 
	UFUNCTION(BlueprintCallable, Category = "Blur ARPG Framework | Combat")
	bool IsCurrentCombatWeapon(const FGameplayTag InWeaponTag) const;

	/// 开关当前战斗状态下持有的武器的碰撞盒。
	/// @param bShouldEnable 
	UFUNCTION(BlueprintCallable, Category = "Blur ARPG Framework | Combat")
	void ToggleCollisionWithCurrentCombatWeapon(const bool bShouldEnable);

	//** 回调 **//
	virtual void OnMeleeHitTargetActor(AActor* HitActor); // 当武器命中目标时。
	virtual void OnMeleePulledFromTargetActor(AActor* HitActor); // 当武器离开目标时。

protected:
	// 和碰撞盒重叠的目标缓存。
	TArray<TWeakObjectPtr<AActor>> OverlappedActors;
	
private:
	// 当前装备到角色的武器。（注意，不是当前战斗状态下持有的武器）
	TMap<FGameplayTag, ABlurWeapon*> EquippedWeaponsMap;
};
