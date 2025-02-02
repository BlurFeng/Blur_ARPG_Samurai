// Blur Feng All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Components/BlurPawnComponentBase.h"
#include "GameplayTagContainer.h"

#include "BlurCombatComponent.generated.h"

class ABlurWeapon;

UENUM(BlueprintType)
enum class EToggleDamageType : uint8
{
	CurrentEquippedWeapon,
	LeftHand,
	RightHand,
};

// 战斗组件基类。
UCLASS()
class BLURARPGFRAMEWORK_API UBlurCombatComponent : public UBlurPawnComponentBase
{
	GENERATED_BODY()

public:
	//当前装备武器Tag。应当在使用“装备武器”技能Ability时修改。
	UPROPERTY(BlueprintReadOnly, Category = "Blur ARPG Framework | Combat")
	FGameplayTag CurrentEquippedWeaponTag;

	UFUNCTION(BlueprintCallable, Category = "Blur ARPG Framework | Combat")
	bool GetWeapon(FGameplayTag InWeaponTag, ABlurWeapon* InWeapon, const bool bRegisterAsEquippedWeapon);

	UFUNCTION(BlueprintCallable, Category = "Blur ARPG Framework | Combat")
	bool DiscardWeapon(const FGameplayTag InWeaponTag);
	
	UFUNCTION(BlueprintCallable, Category = "Blur ARPG Framework | Combat")
	bool Equip(const FGameplayTag InWeaponTag);
	
	UFUNCTION(BlueprintCallable, Category = "Blur ARPG Framework | Combat")
	bool Unequip(const FGameplayTag InWeaponTag);

	UFUNCTION(BlueprintCallable, Category = "Blur ARPG Framework | Combat")
	bool UnequipCurrent();

	/// 注册一个持有武器。当获得武器时调用。
	/// @param InWeaponTag 武器Tag。
	/// @param InWeapon 生成的武器。
	bool RegisterWeapon(const FGameplayTag InWeaponTag, ABlurWeapon* InWeapon);

	bool UnregisterWeapon(const FGameplayTag InWeaponTag);

	/// 获取角色当前持有的武器，通过Tag。
	/// @param InWeaponTagToGet 武器Tag。
	/// @return 
	UFUNCTION(BlueprintCallable, Category = "Blur ARPG Framework | Combat")
	ABlurWeapon* GetCarriedWeaponByTag(const FGameplayTag InWeaponTagToGet) const;

	/// 是持有的武器。
	/// @param InWeaponTag 
	/// @return 
	UFUNCTION(BlueprintCallable, Category = "Blur ARPG Framework | Combat")
	bool IsCarriedWeapon(const FGameplayTag InWeaponTag) const;

	/// 获取角色当前装备的武器。
	/// @return 
	UFUNCTION(BlueprintCallable, Category = "Blur ARPG Framework | Combat")
	ABlurWeapon* GetCurrentEquippedWeapon() const;

	/// 确认是否是当前持有武器。 
	/// @param InWeaponTag 
	/// @return 
	UFUNCTION(BlueprintCallable, Category = "Blur ARPG Framework | Combat")
	bool IsCurrentEquippedWeapon(const FGameplayTag InWeaponTag) const;

	/// 开关武器碰撞盒
	/// @param bShouldEnable 
	/// @param ToggleDamageType 
	UFUNCTION(BlueprintCallable, Category = "Blur ARPG Framework | Combat")
	void ToggleWeaponCollision(const bool bShouldEnable,const EToggleDamageType ToggleDamageType = EToggleDamageType::CurrentEquippedWeapon);

	//** 回调 **//
	virtual void OnMeleeHitTargetActor(AActor* HitActor); //当武器命中目标时
	virtual void OnMeleePulledFromTargetActor(AActor* HitActor); //当武器离开目标时

protected:
	virtual void ToggleCurrentEquippedWeaponCollision(const bool bShouldEnable);
	virtual void ToggleBodyCollisionBoxCollision(const bool bShouldEnable, const EToggleDamageType ToggleDamageType);
	
	TArray<TWeakObjectPtr<AActor>> OverlappedActors;
	
private:
	TMap<FGameplayTag, ABlurWeapon*> CarriedWeaponsMap;
};
