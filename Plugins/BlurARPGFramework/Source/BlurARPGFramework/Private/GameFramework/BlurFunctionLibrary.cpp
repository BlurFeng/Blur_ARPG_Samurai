// Blur Feng All Rights Reserved.


#include "GameFramework/BlurFunctionLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/GameplayAbilitySystem/BlurAbilitySystemComponentBase.h"

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
