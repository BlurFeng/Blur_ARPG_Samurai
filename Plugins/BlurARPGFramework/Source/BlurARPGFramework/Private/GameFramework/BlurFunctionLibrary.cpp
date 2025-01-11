// Blur Feng All Rights Reserved.


#include "GameFramework/BlurFunctionLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/GameplayAbilitySystem/BlurAbilitySystemComponentBase.h"

int32 UBlurFunctionLibrary::RandomIndexByWeights(const TArray<int32>& Weights, int32 WeightTotal)
{
	if (Weights.IsEmpty()) return 0;

	// 没有传入有效总权重时，自行计算。
	if (WeightTotal <= 0)
	{
		WeightTotal = 0;
		for (int32 i = 0; i < Weights.Num(); i++)
		{
			WeightTotal += Weights[i];
		}
	}

	// 随机数在总权重值内。
	const int32 RandomInt = FMath::RandRange(1, WeightTotal);

	// 确认随机数命中区段。
	int32 Right = 0;
	for (int32 i = 0; i < Weights.Num(); i++)
	{
		Right += Weights[i];

		if (RandomInt <= Right)
		{
			return i;
		}
	}

	return 0;
}

int32 UBlurFunctionLibrary::RandomIndexByWeightsForThree(const int32 Weight1, const int32 Weight2, const int32 Weight3)
{
	const int32 WeightTotal = Weight1 + Weight2 + Weight3;
	const int32 RandomInt = FMath::RandRange(1, WeightTotal);

	// 确认随机数命中区段。
	if (RandomInt <= Weight1) return 0;
	if (RandomInt <= Weight1 + Weight2) return 1;
	return 2;
}

float UBlurFunctionLibrary::LerpLimitChangeMin(const float A, const float B, const float LimitChangeMin,
	const float Alpha)
{
	if (B == A) return B;
	
	float Change = Alpha * (B - A);

	// 变化小于限制最小变化值。
	if (LimitChangeMin > 0 && FMath::Abs(Change) < LimitChangeMin)
	{
		if ( Change > 0.f )
		{
			if (A + LimitChangeMin <= B)
			{
				Change = LimitChangeMin;
			}
			else
			{
				return B;
			}
		}
		else
		{
			if (A + LimitChangeMin >= B)
			{
				Change = -LimitChangeMin;
			}
			else
			{
				return B;
			}
		}
	}
	
	return A + Change;
}

ETeamAttitudeFlags UBlurFunctionLibrary::CheckTeamAttitude(const APawn* QueryPawn, const APawn* TargetPawn)
{
	check(QueryPawn && TargetPawn);
	
	const IGenericTeamAgentInterface* QueryTeamAgent = Cast<IGenericTeamAgentInterface>(QueryPawn->GetController());
	const IGenericTeamAgentInterface* TargetTeamAgent = Cast<IGenericTeamAgentInterface>(TargetPawn->GetController());

	if (QueryTeamAgent && TargetTeamAgent)
	{
		// 自身队伍ID和目标队伍ID不相等。
		return QueryTeamAgent->GetGenericTeamId() == TargetTeamAgent->GetGenericTeamId() ?
			ETeamAttitudeFlags::Friendly : ETeamAttitudeFlags::Hostile;
	}

	//将没有 TeamId 的认定为中立。
	//Tips：如果想给中立分配一个Id，这里的代码需要调整。
	return ETeamAttitudeFlags::Neutral;
}

bool UBlurFunctionLibrary::IsValidBlock(const AActor* InAttacker, const AActor* InDefender, const float RightCheckNum,
	const EBlurComparisonOp Comparison)
{
	check(InAttacker && InDefender);

	const float DotResult = FVector::DotProduct(InAttacker->GetActorForwardVector(), InDefender->GetActorForwardVector());
	
	switch (Comparison)
	{
	case EBlurComparisonOp::Equals:
		return DotResult == RightCheckNum;
	case EBlurComparisonOp::NotEquals:
		return DotResult != RightCheckNum;
	case EBlurComparisonOp::GreaterThan:
		return DotResult > RightCheckNum;
	case EBlurComparisonOp::GreaterThanEquals:
		return DotResult >= RightCheckNum;
	case EBlurComparisonOp::LessThan:
		return DotResult < RightCheckNum;
	case EBlurComparisonOp::LessThanEquals:
		return DotResult <= RightCheckNum;
	}
	
	return false;
}

UBlurAbilitySystemComponentBase* UBlurFunctionLibrary::GetAbilitySystemComponentFromActor(AActor* InActor)
{
	// tips：在技能调用中按ESC结束游戏会导致空。
	// check(InActor);
	if (!InActor) return nullptr;

	return CastChecked<UBlurAbilitySystemComponentBase>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InActor));
}

bool UBlurFunctionLibrary::ActorHasMatchingGameplayTag(AActor* InActor, const FGameplayTag TagToCheck)
{
	const UBlurAbilitySystemComponentBase* AbilitySystemComponent = GetAbilitySystemComponentFromActor(InActor);

	return AbilitySystemComponent && AbilitySystemComponent->HasMatchingGameplayTag(TagToCheck);
}

bool UBlurFunctionLibrary::ActorHasAllMatchingGameplayTags(AActor* InActor, const FGameplayTagContainer& TagContainer)
{
	const UBlurAbilitySystemComponentBase* AbilitySystemComponent = GetAbilitySystemComponentFromActor(InActor);

	return AbilitySystemComponent && AbilitySystemComponent->HasAllMatchingGameplayTags(TagContainer);
}

bool UBlurFunctionLibrary::ActorHasAnyMatchingGameplayTags(AActor* InActor, const FGameplayTagContainer& TagContainer)
{
	const UBlurAbilitySystemComponentBase* AbilitySystemComponent = GetAbilitySystemComponentFromActor(InActor);

	return AbilitySystemComponent && AbilitySystemComponent->HasAnyMatchingGameplayTags(TagContainer);
}

bool UBlurFunctionLibrary::ApplyGameplayEffectSpecHandleToTargetActor(AActor* InInstigator, AActor* InTargetActor,
	FGameplayEffectSpecHandle& InSpecHandle)
{
	UBlurAbilitySystemComponentBase* SourceAbilitySystemComponent = GetAbilitySystemComponentFromActor(InInstigator);
	UBlurAbilitySystemComponentBase* TargetAbilitySystemComponent = GetAbilitySystemComponentFromActor(InTargetActor);
	if (!SourceAbilitySystemComponent || !TargetAbilitySystemComponent) return false;
	
	const FActiveGameplayEffectHandle ActiveGameplayEffectHandle = SourceAbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*InSpecHandle.Data, TargetAbilitySystemComponent);

	return ActiveGameplayEffectHandle.WasSuccessfullyApplied();
}
