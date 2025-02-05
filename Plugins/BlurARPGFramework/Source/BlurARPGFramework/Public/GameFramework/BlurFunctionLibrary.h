// Blur Feng All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayAbilitySpec.h"
#include "GameplayEffectTypes.h"
#include "Types/BlurEnumTypes.h"
#include "Types/BlurStructTypes.h"

#include "BlurFunctionLibrary.generated.h"

enum class EBlurComparisonOp : uint8;
class UBlurAbilitySystemComponent;

// 常用方法库。
UCLASS()
class BLURARPGFRAMEWORK_API UBlurFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

#pragma region Common

	// Notes：Latent Action 潜在事件节点。
	// 通过 Latent Action 我们可以实现一些蓝图的异步节点。比如常用的 Delay 就是一个 Latent Action。
	// Notes：元数据说明符。
	// Latent：标记此方法为 Latent 方法。
	// LatentInfo：此方法必须有 FLatentActionInfo 参数并通过 LatentInfo 标记。
	// WorldContext：自动获得 WorldContextObject 参数。当我们的方法需要 世界 信息时，可以通过此方式自动获取。在生成Actor，计时器，全局设置等时候。
	// UPARAM(DisplayName = "Output")：我们可以在参数前使用UPARAM()进行说明。这里重设了显示名称以防止过长的文字。

	/// 倒计时异步方法。
	/// @param WorldContextObject 
	/// @param TotalTime 总时间。为负数时会一直执行直到取消。
	/// @param UpdateInterval 更新间隔。
	/// @param ExecuteOnFirst 在开始的第一帧执行。
	/// @param PausedWithGame 随游戏暂停计时。
	/// @param OutRemainingTime 剩余时间。
	/// @param OutDeltaTime 每帧变化时间。（它不等于 UpdateInterval 时间。）
	/// @param CountDownInput 输入执行引脚。
	/// @param CountDownOutput 输出执行引脚。
	/// @param LatentInfo 
	UFUNCTION(BlueprintCallable, Category = "Warrior|FunctionLibrary", meta = (Latent, WorldContext = "WorldContextObject", LatentInfo = "LatentInfo", ExpandEnumAsExecs = "CountDownInput|CountDownOutput", TotalTime = "1.0", UpdateInterval = "0.1", ExecuteOnFirst = "true", PausedWithGame = "true"))
	static void CountDown(const UObject* WorldContextObject, const float TotalTime, const float UpdateInterval, const bool ExecuteOnFirst, const bool PausedWithGame,
		float& OutRemainingTime,  float& OutDeltaTime, const EBlurCountDownActionInput CountDownInput, UPARAM(DisplayName = "Output") EBlurCountDownActionOutput& CountDownOutput,
		const FLatentActionInfo LatentInfo);

	/// 根据传入的权重数组，根据权重随机并返回一个Index。
	/// @param Weights 权重数组。 
	/// @param WeightTotal 如果知道总权重，传入参数可以减少计算，否则自动计算总权重。
	/// @return 随机选中的权重Index。
	UFUNCTION(BlueprintPure, Category = "Blur ARPG Framework | Function Library")
	static int32 RandomIndexByWeights(const TArray<int32>& Weights, int32 WeightTotal = 0);

	/// 在三个权重值之间随机并返回对应Index。
	/// @param Weight1 
	/// @param Weight2 
	/// @param Weight3 
	/// @return 
	UFUNCTION(BlueprintPure, Category = "Blur ARPG Framework | Function Library")
	static int32 RandomIndexByWeightsForThree(const int32 Weight1, const int32 Weight2, const int32 Weight3);

	/// Lerp插值，变化值不小于LimitChangeMin。
	/// @param A 当前值。
	/// @param B 目标值。
	/// @param LimitChangeMin 限制变化最小值。>0时有效。
	/// @param Alpha 
	/// @return 
	UFUNCTION(BlueprintPure, Category = "Blur ARPG Framework | Function Library")
	static float LerpLimitChangeMin(const float A, const float B, const float LimitChangeMin, const float Alpha);
	
#pragma endregion
	
#pragma region Gameplay
	
	/// 确认目标Pawn是否是敌对的。
	/// @param QueryPawn 
	/// @param TargetPawn 
	/// @return 
	UFUNCTION(BlueprintPure, Category = "Blur ARPG Framework | Function Library | Gameplay")
	static ETeamAttitudeFlags CheckTeamAttitude(const APawn* QueryPawn, const APawn* TargetPawn);

	/// 确认格挡是否有效。
	/// 将两者的 ForwardVector 进行 Dot 并和 RightCheckNum 进行比较。
	/// @param InAttacker 
	/// @param InDefender 
	/// @param RightCheckNum 
	/// @param Comparison 
	/// @return 
	UFUNCTION(BlueprintPure, Category = "Blur ARPG Framework | Function Library | Gameplay")
	static bool IsValidBlock(const AActor* InAttacker, const AActor* InDefender, const float RightCheckNum = -0.2f, const EBlurComparisonOp Comparison = EBlurComparisonOp::LessThanEquals);

#pragma endregion

#pragma region Gameplay Ability System

	/// 获取AbilitySystemComponent从Actor。
	/// @param InActor 
	/// @return
	UFUNCTION(BlueprintPure, Category = "Blur ARPG Framework | Function Library | Gameplay Ability System")
	static UBlurAbilitySystemComponent* GetAbilitySystemComponentFromActor(AActor* InActor);

	/// 向Actor（的技能组件）添加一个还未包含的Tag。
	/// @param InActor 目标Actor。
	/// @param TagToAdd 添加Tag。
	UFUNCTION(BlueprintCallable, Category = "Blur ARPG Framework | Warrior | Function Library")
	static void AddGameplayTagToActorIfNone(AActor* InActor, FGameplayTag TagToAdd);

	/// 移除一个Tag从Actor。
	/// @param InActor 目标Actor。
	/// @param TagToRemove 移除Tag。
	UFUNCTION(BlueprintCallable, Category = "Blur ARPG Framework | Warrior | Function Library")
	static void RemoveGameplayTagFromActorIfFound(AActor* InActor, FGameplayTag TagToRemove);

	/// 确认Actor（的技能组件）是否包含Tag。
	/// @param InActor 目标Actor。
	/// @param TagToCheck 确认Tag。
	/// @return
	UFUNCTION(BlueprintPure, Category = "Blur ARPG Framework | Function Library | Gameplay Ability System")
	static bool ActorHasMatchingGameplayTag(AActor* InActor, const FGameplayTag TagToCheck);

	/// Actor拥有所有的Tag。
	/// @param InActor 
	/// @param TagContainer 
	/// @return
	UFUNCTION(BlueprintPure, Category = "Blur ARPG Framework | Function Library | Gameplay Ability System")
	static bool ActorHasAllMatchingGameplayTags(AActor* InActor, const FGameplayTagContainer& TagContainer);

	/// Actor拥有任一的Tag。
	/// @param InActor 
	/// @param TagContainer 
	/// @return
	UFUNCTION(BlueprintPure, Category = "Blur ARPG Framework | Function Library | Gameplay Ability System")
	static bool ActorHasAnyMatchingGameplayTags(AActor* InActor, const FGameplayTagContainer& TagContainer);

	/// 将GE应用到目标Actor上。
	/// @param InInstigator 施法者。
	/// @param InTargetActor 目标Actor。
	/// @param InSpecHandle GE说明句柄。
	/// @return 
	UFUNCTION(BlueprintCallable, Category = "Blur ARPG Framework | Function Library | Gameplay Ability System")
	static bool ApplyGameplayEffectSpecHandleToTargetActor(AActor* InInstigator, AActor* InTargetActor, FGameplayEffectSpecHandle& InSpecHandle);

	/// 创建一个 FGameplayAbilitySpec。 Gameplay Ability Specification 用于描述Ability技能的详细信息。
	/// @param GameplayAbility 技能。
	/// @param SourceObject 来源。比如施法者。
	/// @param ApplyLevel 技能等级。可用于在配置表查询不同等级对应的不同数值。
	/// @return 
	static FGameplayAbilitySpec GetGameplayAbilitySpec(const TSubclassOf<UGameplayAbility>& GameplayAbility, const TWeakObjectPtr<UObject> SourceObject, const int32 ApplyLevel);

	/// 创建一个 FGameplayAbilitySpec。 Gameplay Ability Specification 用于描述Ability技能的详细信息。
	/// @param GameplayAbility 技能。
	/// @param SourceObject 来源。比如施法者。
	/// @param ApplyLevel 技能等级。可用于在配置表查询不同等级对应的不同数值。
	/// @param InputTag 添加到 DynamicAbilityTags 的Tag。可在之后作为查询的数据。
	/// @return 
	static FGameplayAbilitySpec GetGameplayAbilitySpec(const TSubclassOf<UGameplayAbility>& GameplayAbility, const TWeakObjectPtr<UObject> SourceObject, const int32 ApplyLevel, const FGameplayTag InputTag);

	/// 从ScalableFloat获取Float，根据Level。
	/// @param InScalableFloat 
	/// @param InLevel 
	/// @return 
	UFUNCTION(BlueprintPure, Category = "Blur ARPG Framework | Function Library | Gameplay Ability System", meta = (CompactNodeTitle = "Get Value At Level"))
	static float GetScalableFloatValueAtLevel(const FScalableFloat& InScalableFloat, const float InLevel);

	/// 获取技能剩余冷却时间。
	/// @param OwnerActor 所有者Actor，然后尝试从Actor获取他的技能组件。 
	/// @param InCooldownTag 冷却GETag。 
	/// @param TotalCooldownTime 总时间。
	/// @param RemainingCooldownTime 剩余时间。
	/// @return 
	static bool GetAbilityCooldownByTag(AActor* OwnerActor, const FGameplayTag InCooldownTag, float& TotalCooldownTime, float& RemainingCooldownTime);

	/// 获取技能剩余冷却时间。
	/// @param AbilitySystemComponent 技能组件。 
	/// @param InCooldownTag 冷却GETag。 
	/// @param TotalCooldownTime 总时间。
	/// @param RemainingCooldownTime 剩余时间。
	/// @return 
	UFUNCTION(BlueprintPure, Category = "Blur ARPG Framework | Function Library | Gameplay Ability System")
	static bool GetAbilityCooldownByTag(const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag InCooldownTag, float& TotalCooldownTime, float& RemainingCooldownTime);
	
#pragma endregion
};
