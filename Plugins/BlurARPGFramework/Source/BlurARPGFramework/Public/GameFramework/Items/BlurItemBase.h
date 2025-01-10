// Blur Feng All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/BlurActorBase.h"
#include "GameplayTagContainer.h"
#include "GameplayTagAssetInterface.h"

#include "BlurItemBase.generated.h"

// 道具物品基类。
UCLASS()
class BLURARPGFRAMEWORK_API ABlurItemBase : public ABlurActorBase, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

public:
	ABlurItemBase();

	//~ Begin IGameplayTagAssetInterface Interface.
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;
	// 接口内部已经做了实现，除非有其他额外需求时，重写以下接口。
	// virtual bool HasMatchingGameplayTag(FGameplayTag TagToCheck) const override;
	// virtual bool HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const override;
	// virtual bool HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const override;
	//~ End IGameplayTagAssetInterface Interface

	/// 添加Tag。
	/// 添加已经存在的Tag并不能叠加。
	/// @param Tag 
	void AddGameplayTag(const FGameplayTag& Tag);

	/// 移除Tag。
	/// @param Tag 
	UFUNCTION(BlueprintCallable, Category = "Item")
	void RemoveGameplayTag(const FGameplayTag& Tag);

protected:

	// Tips：
	// 我们可以用Tag对Item进行分类，达到不同的游戏需求。
	// 比如释放一个技能，将所有的瓦罐击碎。比起确认Class，Tag是更加易用的方式。
	
	// 游戏Tag组件，用于设置查询Tag。
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FGameplayTagContainer GameplayTagContainer;
	
};
