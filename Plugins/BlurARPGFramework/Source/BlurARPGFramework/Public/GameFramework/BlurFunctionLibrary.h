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
class UBlurAbilitySystemComponentBase;

// 常用方法库。
UCLASS()
class BLURARPGFRAMEWORK_API UBlurFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

#pragma region Common

	/// 根据传入的权重数组，根据权重随机并返回一个Index。
	/// @param Weights 权重数组。 
	/// @param WeightTotal 如果知道总权重，传入参数可以减少计算，否则自动计算总权重。
	/// @return 随机选中的权重Index。
	UFUNCTION(BlueprintPure, Category = "Warrior|FunctionLibrary")
	static int32 RandomIndexByWeights(const TArray<int32>& Weights, int32 WeightTotal = 0);

	/// 在三个权重值之间随机并返回对应Index。
	/// @param Weight1 
	/// @param Weight2 
	/// @param Weight3 
	/// @return 
	UFUNCTION(BlueprintPure, Category = "Warrior|FunctionLibrary")
	static int32 RandomIndexByWeightsForThree(const int32 Weight1, const int32 Weight2, const int32 Weight3);

	/// Lerp插值，变化值不小于LimitChangeMin。
	/// @param A 当前值。
	/// @param B 目标值。
	/// @param LimitChangeMin 限制变化最小值。>0时有效。
	/// @param Alpha 
	/// @return 
	UFUNCTION(BlueprintPure, Category = "Warrior|FunctionLibrary")
	static float LerpLimitChangeMin(const float A, const float B, const float LimitChangeMin, const float Alpha);
	
#pragma endregion
	
#pragma region Gameplay
	
	/// 确认目标Pawn是否是敌对的。
	/// @param QueryPawn 
	/// @param TargetPawn 
	/// @return 
	UFUNCTION(BlueprintPure, Category = "BlurARPGFramework|FunctionLibrary|Gameplay")
	static ETeamAttitudeFlags CheckTeamAttitude(const APawn* QueryPawn, const APawn* TargetPawn);

	/// 确认格挡是否有效。
	/// 将两者的 ForwardVector 进行 Dot 并和 RightCheckNum 进行比较。
	/// @param InAttacker 
	/// @param InDefender 
	/// @param RightCheckNum 
	/// @param Comparison 
	/// @return 
	UFUNCTION(BlueprintPure, Category = "BlurARPGFramework|FunctionLibrary|Gameplay")
	static bool IsValidBlock(const AActor* InAttacker, const AActor* InDefender, const float RightCheckNum = -0.2f, const EBlurComparisonOp Comparison = EBlurComparisonOp::LessThanEquals);

#pragma endregion

#pragma region Gameplay Ability System

	/// 获取AbilitySystemComponent从Actor。
	/// @param InActor 
	/// @return
	UFUNCTION(BlueprintPure, Category = "BlurARPGFramework|FunctionLibrary|GameplayAbilitySystem")
	static UBlurAbilitySystemComponentBase* GetAbilitySystemComponentFromActor(AActor* InActor);

	/// 确认Actor（的技能组件）是否包含Tag。
	/// @param InActor 目标Actor。
	/// @param TagToCheck 确认Tag。
	/// @return
	UFUNCTION(BlueprintPure, Category = "BlurARPGFramework|FunctionLibrary|GameplayAbilitySystem")
	static bool ActorHasMatchingGameplayTag(AActor* InActor, const FGameplayTag TagToCheck);

	/// Actor拥有所有的Tag。
	/// @param InActor 
	/// @param TagContainer 
	/// @return
	UFUNCTION(BlueprintPure, Category = "BlurARPGFramework|FunctionLibrary|GameplayAbilitySystem")
	static bool ActorHasAllMatchingGameplayTags(AActor* InActor, const FGameplayTagContainer& TagContainer);

	/// Actor拥有任一的Tag。
	/// @param InActor 
	/// @param TagContainer 
	/// @return
	UFUNCTION(BlueprintPure, Category = "BlurARPGFramework|FunctionLibrary|GameplayAbilitySystem")
	static bool ActorHasAnyMatchingGameplayTags(AActor* InActor, const FGameplayTagContainer& TagContainer);

	/// 将GE应用到目标Actor上。
	/// @param InInstigator 施法者。
	/// @param InTargetActor 目标Actor。
	/// @param InSpecHandle GE说明句柄。
	/// @return 
	UFUNCTION(BlueprintCallable, Category = "BlurARPGFramework|FunctionLibrary|GameplayAbilitySystem")
	static bool ApplyGameplayEffectSpecHandleToTargetActor(AActor* InInstigator, AActor* InTargetActor, FGameplayEffectSpecHandle& InSpecHandle);
	
#pragma endregion
};
