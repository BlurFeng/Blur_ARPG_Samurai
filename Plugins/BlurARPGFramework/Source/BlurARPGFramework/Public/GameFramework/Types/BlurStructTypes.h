// Blur Feng All Rights Reserved.

#pragma once

// Notes：Error C2665 : “FGameplayAbilitySpec::FGameplayAbilitySpec”: 没有重载函数可以转换所有参数类型。
// 使用 #include "GameFramework/GameplayAbilitySystem/Abilities/BlurGameplayAbilityBase.h" 完整引用 代替 class UBlurGameplayAbilityBase 前置引用来解决这个问题。
// 否则在其他类中引用 BlurStructTypes.h 并使用其中的数据结构，并用到这些 前置引用的类 时可能出现问题。
// 比如一个方法要求传入 class A，但你 传入了他的子类 class A1，但 A1 是前置引用的，所以方法并不知道 A1 是 A 的子类，导致方法无法使用。如果他是重载方法，就会报错没找到可用的重载方法。
// 为了安全起见，我们总是为我们自己创造的 子类 进行#include完整引用。
// 一些UE引擎原生的类，我们依然可以使用 前置引用。或者这个类在蓝图中使用时，并不会有相关的报错产生。
// 还有一点需要注意，我们使用 增量编译 时，这样的问题可能被隐藏，知道下一次被编译（修改内容或完整重编译）时才会暴露此问题。

#include "GameFramework/GameplayAbilitySystem/Abilities/BlurGameplayAbility.h"
#include "GameplayTagContainer.h"
#include "ScalableFloat.h"

// Notes：generated.h
// 在UE中，我们必须需要 #include "BlurStructTypes.generated.h" 才能成功编译这个文件。
// 每个包含 GENERATED_BODY() 或类似宏（如 GENERATED_UCLASS_BODY）的类或结构，必须在对应的头文件中包含一个 .generated.h 文件。
// 编译顺序依赖： Unreal Engine 的代码生成是一个多阶段的编译过程，generated.h 文件的内容在普通编译器处理之前由 UHT 生成，缺少它会导致 GENERATED_BODY() 无法展开。
// 包含 #include "BlurStructTypes.generated.h" 是使用 Unreal Engine 特定功能（如反射、蓝图支持、序列化）的必需步骤。
// 确保 .generated.h 文件正确生成和引用是 USTRUCT、UCLASS 和 UENUM 正常运行的核心前提。

#include "BlurStructTypes.generated.h"

class ABlurWeaponObject;
class UBlurLinkedAnimLayer;
class UInputMappingContext;

// 给与技能配置。
USTRUCT(BlueprintType)
struct FGiveAbilitySet
{
	GENERATED_BODY()

	// 输入Tag。决定了这个技能是怎样被使用激活的。
	// 如果是确定不会被玩家操控的角色，可以不配置此Tag。
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "InputTag"))
	FGameplayTag InputTag;
	
	// 赋予技能。
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UBlurGameplayAbility> AbilityToGive;
	
	bool IsValid() const;

#if WITH_EDITOR
	void PostEditChangeProperty();
#endif
};

// 特殊技能配置。
USTRUCT(BlueprintType)
struct FGiveSpecialAbilitySet : public FGiveAbilitySet
{
	GENERATED_BODY()

	// 技能Icon材质。
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UMaterialInterface> SoftAbilityIconMaterial;

	// 技能冷却Tag。
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "Ability.Cooldown"))
	FGameplayTag AbilityCooldownTag;
};

USTRUCT(BlueprintType)
struct FWeaponObjectInfo
{
	GENERATED_BODY()

	// 武器部件名称。可以用于查询。
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName Name;
	
	// 一个武器生成的Actor部件。比如生成的刀和刀鞘。
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<ABlurWeaponObject> WeaponObjectClass;

	// 附加到的骨骼插槽名称。
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName AttachToSocketName;
};

USTRUCT(BlueprintType)
struct FWeaponInfo
{
	GENERATED_BODY()
	
	// 武器拥有的部件Actor。比如刀和刀鞘。他们在武器被装备到角色时生成，并附加到对应的骨骼位置。
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FWeaponObjectInfo> WeaponObjectInfos;
	
	// 武器输入映射。当装备武器时，使用此InputMap覆盖原有的。因为装备武器后操作方式会相应变化。
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputMappingContext* WeaponInputMappingContext;

	// 武器输入映射的优先级，应当高于角色默认的输入映射优先级。
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int WeaponInputMappingPriority = 1;

	// 武器的链接动画层，Link到角色Mesh来改变动画表现。在武器装备时。
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UBlurLinkedAnimLayer> AnimLayerForEquip;

	// 武器的链接动画层，Link到角色Mesh来改变动画表现。在使用武器进入战斗状态时。
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UBlurLinkedAnimLayer> AnimLayerForEnterCombat;

	// 使用曲线设置武器的基础伤害。
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FScalableFloat WeaponBaseDamage;
	
	// 武器图标Icon。
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftObjectPtr<UTexture2D> SoftWeaponIconTexture;

	// 武器Actor。用于创建武器模型Actor并赋予
};

USTRUCT(BlueprintType)
struct FAbilityWeaponInfo
{
	GENERATED_BODY()

	// 武器默认技能等级。一般使用武器的等级来设置技能等级，当没有有效的武器等级时，使用默认等级。
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int DefaultWeaponAbilityLevel = 1;
	
	// 装备武器技能组。当把武器从背包装备到角色身上时获得的技能。一般会包含 拔出武器 和 收回武器 两个技能。
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputTag"))
	TArray<FGiveAbilitySet> AbilitiesWithEquip;
	
	// 战斗技能组。当调用 EnterCombat 进入战斗状态时，此技能会被赋予角色。一般包含 攻击技能 和 防御技能。
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputTag"))
	TArray<FGiveAbilitySet> AbilitiesWithCombat;
	
	// 战斗特殊技能组。当调用 EnterCombat 进入战斗状态时，此技能会被赋予角色。强力的技能，一般有cost 和 CD。
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputTag"))
	TArray<FGiveSpecialAbilitySet> SpecialAbilitiesWithCombat;
};