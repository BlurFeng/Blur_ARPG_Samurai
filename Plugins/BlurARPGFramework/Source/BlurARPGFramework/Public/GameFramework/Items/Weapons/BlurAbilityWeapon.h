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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "WeaponData")
	FAbilityWeaponData AbilityWeaponData;

	UFUNCTION(BlueprintCallable)
	void HandleEquipWeapon(UBlurAbilitySystemComponent* AbilitySystemComponent, USkeletalMeshComponent* SkeletalMeshComponent = nullptr, APlayerController* PlayerController = nullptr);

	/// 将赋予所有者技能时返回的 Handle 缓存，用于之后移除技能。
	/// @param InSpecHandles 
	UFUNCTION(BlueprintCallable)
	void AssignGrantedAbilitySpecHandles(const TArray<FGameplayAbilitySpecHandle>& InSpecHandles);

	/// 获取 武器赋予技能时返回的句柄。可用此句柄移除技能。
	/// @return 
	UFUNCTION(BlueprintCallable)
	TArray<FGameplayAbilitySpecHandle> GetGaveAbilitySpecHandles() const;
private:
	
	TArray<FGameplayAbilitySpecHandle> GaveAbilitySpecHandles;
};
