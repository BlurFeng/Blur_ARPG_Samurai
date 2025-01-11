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
