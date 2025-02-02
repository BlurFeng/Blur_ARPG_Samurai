// Blur Feng All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Components/UI/BlurPawnUIComponent.h"
#include "GameplayTagContainer.h"

#include "BlurCharacterUIComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEquippedWeaponChangedDelegate, TSoftObjectPtr<UTexture2D>, SoftWeaponIcon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAbilityIconSlotUpdatedDelegate, FGameplayTag, AbilityInputTag, TSoftObjectPtr<UMaterialInterface>, SoftAbilityIconMaterial);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnAbilityCooldownBeginDelegate, FGameplayTag, AbilityInputTag, float, TotalCooldownTime, float, RemainingCooldownTime);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnStoneInteractedDelegate, bool, bShouldDisplayInputKey);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnCheckCostOrCooldown, bool, bAllow, FGameplayTag, AbilityTag);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSimpleUIEvent);

// 角色UI组件。当角色显示的数据比较复杂时，可以使用此组件。比如玩家控制的角色。或者一个需要显示详细信息的目标。
UCLASS()
class BLURARPGFRAMEWORK_API UBlurCharacterUIComponent : public UBlurPawnUIComponent
{
	GENERATED_BODY()

public:
	
	// 当前体力值变化时。
	UPROPERTY(BlueprintAssignable)
	FOnPercentChangedDelegate OnStaminaChanged;
	
	// 当前怒气值变化时。
	UPROPERTY(BlueprintAssignable)
	FOnPercentChangedDelegate OnRageChanged;
	
	// 当装备武器变化时。
	UPROPERTY(BlueprintAssignable)
	FOnEquippedWeaponChangedDelegate OnEquippedWeaponChangedDelegate;

	// 当取消一个技能时。
	UPROPERTY(BlueprintAssignable)
	FOnCheckCostOrCooldown OnCancelAbility;

	// 当确认技能Cooldown是否足够时。
	UPROPERTY(BlueprintAssignable)
	FOnCheckCostOrCooldown OnCheckCooldown;

	// 当确认技能Cost是否足够时。
	UPROPERTY(BlueprintAssignable)
	FOnCheckCostOrCooldown OnCheckCost;
	
	// 当技能Icon槽更新时。
	UPROPERTY(BlueprintAssignable)
	FOnAbilityIconSlotUpdatedDelegate OnAbilityIconSlotUpdatedDelegate;

	// 当技能CD开始显示时。
	UPROPERTY(BlueprintAssignable)
	FOnAbilityCooldownBeginDelegate OnAbilityCooldownBegin;

	// 当技能CD取消显示时。我们停止计数器防止额外的计算消耗。
	UPROPERTY(BlueprintAssignable)
	FSimpleUIEvent OnAbilityCooldownCancel;

	// //当和石头交互时。
	// UPROPERTY(BlueprintCallable, BlueprintAssignable)
	// FOnStoneInteractedDelegate OnStoneInteracted;






};
