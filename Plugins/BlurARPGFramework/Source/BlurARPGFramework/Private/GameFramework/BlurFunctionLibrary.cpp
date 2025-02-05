// Blur Feng All Rights Reserved.


#include "GameFramework/BlurFunctionLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GenericTeamAgentInterface.h"
#include "GameFramework/Common/BlurCountDownAction.h"
#include "GameFramework/GameplayAbilitySystem/BlurAbilitySystemComponent.h"

void UBlurFunctionLibrary::CountDown(const UObject* WorldContextObject, const float TotalTime, const float UpdateInterval,
	const bool ExecuteOnFirst, const bool PausedWithGame, float& OutRemainingTime, float& OutDeltaTime,
	const EBlurCountDownActionInput CountDownInput, EBlurCountDownActionOutput& CountDownOutput, const FLatentActionInfo LatentInfo)
{
	// 确认World可用。
	UWorld* World = nullptr;
	if (GEngine)
	{
		World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	}
	if (!World) return;

	// 查找LatentAction。
	FLatentActionManager& LatentActionManager = World->GetLatentActionManager();
	FBlurCountDownAction* FoundAction = LatentActionManager.FindExistingAction<FBlurCountDownAction>(LatentInfo.CallbackTarget, LatentInfo.UUID);

	// 输入执行为开始。
	if (CountDownInput == EBlurCountDownActionInput::Start)
	{
		// 创建LatentAction。
		// 此处 new 的类将由 LatentActionManager 进行管理，我们无需担心内存泄露问题。
		if (!FoundAction)
		{
			LatentActionManager.AddNewAction(
				LatentInfo.CallbackTarget,
				LatentInfo.UUID, new FBlurCountDownAction(WorldContextObject, TotalTime, UpdateInterval, ExecuteOnFirst, PausedWithGame, OutRemainingTime, OutDeltaTime, CountDownOutput, LatentInfo)
				);
		}
	}

	// 输入执行为取消。
	if (CountDownInput == EBlurCountDownActionInput::Cancel)
	{
		// 取消LatentAction。
		if (FoundAction)
		{
			FoundAction->CancelAction();
		}
	}
}

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

UBlurAbilitySystemComponent* UBlurFunctionLibrary::GetAbilitySystemComponentFromActor(AActor* InActor)
{
	// tips：在技能调用中按ESC结束游戏会导致空。
	// check(InActor);
	if (!InActor) return nullptr;

	return CastChecked<UBlurAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InActor));
}

void UBlurFunctionLibrary::AddGameplayTagToActorIfNone(AActor* InActor, FGameplayTag TagToAdd)
{
	UBlurAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponentFromActor(InActor);

	if(AbilitySystemComponent && !AbilitySystemComponent->HasMatchingGameplayTag(TagToAdd))
	{
		AbilitySystemComponent->AddLooseGameplayTag(TagToAdd);
	}
}

void UBlurFunctionLibrary::RemoveGameplayTagFromActorIfFound(AActor* InActor, FGameplayTag TagToRemove)
{
	UBlurAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponentFromActor(InActor);

	if(AbilitySystemComponent && AbilitySystemComponent->HasMatchingGameplayTag(TagToRemove))
	{
		AbilitySystemComponent->RemoveLooseGameplayTag(TagToRemove);
	}
}

bool UBlurFunctionLibrary::ActorHasMatchingGameplayTag(AActor* InActor, const FGameplayTag TagToCheck)
{
	const UBlurAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponentFromActor(InActor);

	return AbilitySystemComponent && AbilitySystemComponent->HasMatchingGameplayTag(TagToCheck);
}

bool UBlurFunctionLibrary::ActorHasAllMatchingGameplayTags(AActor* InActor, const FGameplayTagContainer& TagContainer)
{
	const UBlurAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponentFromActor(InActor);

	return AbilitySystemComponent && AbilitySystemComponent->HasAllMatchingGameplayTags(TagContainer);
}

bool UBlurFunctionLibrary::ActorHasAnyMatchingGameplayTags(AActor* InActor, const FGameplayTagContainer& TagContainer)
{
	const UBlurAbilitySystemComponent* AbilitySystemComponent = GetAbilitySystemComponentFromActor(InActor);

	return AbilitySystemComponent && AbilitySystemComponent->HasAnyMatchingGameplayTags(TagContainer);
}

bool UBlurFunctionLibrary::ApplyGameplayEffectSpecHandleToTargetActor(AActor* InInstigator, AActor* InTargetActor,
	FGameplayEffectSpecHandle& InSpecHandle)
{
	UBlurAbilitySystemComponent* SourceAbilitySystemComponent = GetAbilitySystemComponentFromActor(InInstigator);
	UBlurAbilitySystemComponent* TargetAbilitySystemComponent = GetAbilitySystemComponentFromActor(InTargetActor);
	if (!SourceAbilitySystemComponent || !TargetAbilitySystemComponent) return false;
	
	const FActiveGameplayEffectHandle ActiveGameplayEffectHandle = SourceAbilitySystemComponent->ApplyGameplayEffectSpecToTarget(*InSpecHandle.Data, TargetAbilitySystemComponent);

	return ActiveGameplayEffectHandle.WasSuccessfullyApplied();
}

FGameplayAbilitySpec UBlurFunctionLibrary::GetGameplayAbilitySpec(const TSubclassOf<UGameplayAbility>& GameplayAbility,
	const TWeakObjectPtr<UObject> SourceObject, const int32 ApplyLevel)
{
	FGameplayAbilitySpec AbilitySpec(GameplayAbility); //技能。
	AbilitySpec.SourceObject = SourceObject; //来源。比如施法者。
	AbilitySpec.Level = ApplyLevel; //技能等级。可用于在配置表查询不同等级对应的不同数值。

	return AbilitySpec;
}

FGameplayAbilitySpec UBlurFunctionLibrary::GetGameplayAbilitySpec(const TSubclassOf<UGameplayAbility>& GameplayAbility,
	const TWeakObjectPtr<UObject> SourceObject, const int32 ApplyLevel, const FGameplayTag InputTag)
{
	FGameplayAbilitySpec AbilitySpec = GetGameplayAbilitySpec(GameplayAbility, SourceObject, ApplyLevel);
	AbilitySpec.GetDynamicSpecSourceTags().AddTag(InputTag);

	return AbilitySpec;
}

float UBlurFunctionLibrary::GetScalableFloatValueAtLevel(const FScalableFloat& InScalableFloat, const float InLevel)
{
	return  InScalableFloat.GetValueAtLevel(InLevel);
}

bool UBlurFunctionLibrary::GetAbilityCooldownByTag(AActor* OwnerActor, const FGameplayTag InCooldownTag,
	float& TotalCooldownTime, float& RemainingCooldownTime)
{
	return GetAbilityCooldownByTag(GetAbilitySystemComponentFromActor(OwnerActor), InCooldownTag, TotalCooldownTime, RemainingCooldownTime);
}

bool UBlurFunctionLibrary::GetAbilityCooldownByTag(
	const UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag InCooldownTag, float& TotalCooldownTime, float& RemainingCooldownTime)
{
	TotalCooldownTime = 0.f;
	RemainingCooldownTime = 0.f;

	if (!AbilitySystemComponent) return false;
	
	check(InCooldownTag.IsValid());

	const FGameplayEffectQuery CooldownQuery = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(InCooldownTag.GetSingleTagContainer());
	
	const TArray<TPair<float, float>> TimeRemainingAndDuration = AbilitySystemComponent->GetActiveEffectsTimeRemainingAndDuration(CooldownQuery);

	if (!TimeRemainingAndDuration.IsEmpty())
	{
		RemainingCooldownTime = TimeRemainingAndDuration[0].Key;
		TotalCooldownTime = TimeRemainingAndDuration[0].Value;
	}

	return RemainingCooldownTime > 0.f;
}