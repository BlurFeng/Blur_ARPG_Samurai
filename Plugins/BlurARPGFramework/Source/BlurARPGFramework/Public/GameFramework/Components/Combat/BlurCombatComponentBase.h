// Blur Feng All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Components/BlurPawnComponentBase.h"
#include "GameplayTagContainer.h"

#include "BlurCombatComponentBase.generated.h"

class ABlurWeapon;

UENUM(BlueprintType)
enum class EToggleDamageType : uint8
{
	CurrentEquippedWeapon,
	LeftHand,
	RightHand,
};

//人物战斗组件基类，角色或敌人的战斗组件由此类派生。
UCLASS()
class BLURARPGFRAMEWORK_API UBlurCombatComponentBase : public UBlurPawnComponentBase
{
	GENERATED_BODY()

public:
	//当前装备武器Tag。应当在使用“装备武器”技能Ability时修改。
	UPROPERTY(BlueprintReadWrite, Category = "BlurARPGFramework|Combat")
	FGameplayTag CurrentEquippedWeaponTag;

	/// 注册一个生成武器。
	/// @param InWeaponTagToRegister 武器Tag。
	/// @param InWeaponToRegister 生成的武器。
	/// @param bRegisterAsEquippedWeapon 注册的武器装备到自身。
	UFUNCTION(BlueprintCallable, Category = "BlurARPGFramework|Combat")
	void RegisterSpawnedWeapon(FGameplayTag InWeaponTagToRegister, ABlurWeapon* InWeaponToRegister, const bool bRegisterAsEquippedWeapon = false);

	/// 获取角色当前持有的武器，通过Tag。
	/// @param InWeaponTagToGet 武器Tag。
	/// @return 
	UFUNCTION(BlueprintCallable, Category = "BlurARPGFramework|Combat")
	ABlurWeapon* GetCarriedWeaponByTag(const FGameplayTag InWeaponTagToGet) const;

	/// 获取角色当前装备的武器。
	/// @return 
	UFUNCTION(BlueprintCallable, Category = "BlurARPGFramework|Combat")
	ABlurWeapon* GetCurrentEquippedWeapon() const;

	/// 开关武器碰撞盒
	/// @param bShouldEnable 
	/// @param ToggleDamageType 
	UFUNCTION(BlueprintCallable, Category = "BlurARPGFramework|Combat")
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
