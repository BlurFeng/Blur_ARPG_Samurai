// Blur Feng All Rights Reserved.


#include "GameFramework/Characters/BlurCharacterBase.h"

#include "GameFramework/GameplayAbilitySystem/BlurAbilitySystemComponent.h"
#include "GameFramework/GameplayAbilitySystem/BlurAttributeSet.h"
#include "MotionWarpingComponent.h"
#include "GameFramework/Common/BlurDebugHelper.h"

// Sets default values
ABlurCharacterBase::ABlurCharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	// 是否接受贴花。关闭。因为会对地面使用一些贴花投射。
	GetMesh()->bReceivesDecals = false;

	// 初始化技能系统。
	BlurAbilitySystemComponent = CreateDefaultSubobject<UBlurAbilitySystemComponent>(TEXT("BlurAbilitySystemComponent"));
	BlurAttributeSet = CreateDefaultSubobject<UBlurAttributeSet>(TEXT("BlurAttributeSet"));

	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComponent"));
}

UAbilitySystemComponent* ABlurCharacterBase::GetAbilitySystemComponent() const
{
	return GetBlurAbilitySystemComponent();
}

UBlurCombatComponent* ABlurCharacterBase::GetPawnCombatComponent() const
{
	return nullptr; // 此接口由子类实现.
}

UBlurPawnUIComponent* ABlurCharacterBase::GetPawnUIComponent() const
{
	return nullptr; // 此接口由子类实现.
}

void ABlurCharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	// 初始化技能组件
	if(BlurAbilitySystemComponent)
	{
		BlurAbilitySystemComponent->InitAbilityActorInfo(this, this);
#if WITH_EDITOR
		if (CharacterStartUpData.IsNull())
		{
			Debug::Print(FString::Printf(TEXT("CharacterStartUpData data is not configured. CharacterName: %s"), *GetActorNameOrLabel()), FColor::Red);
			return;
		}
#else
		// Notes：ensureMsgf()宏方法可以用于检查一些重要，但不至于导致程序崩溃的内容。比如角色的配置数据资源。
		// 检查CharacterStartUpData角色启动数据资源是否正确配置。
		ensureMsgf(!CharacterStartUpData.IsNull(), TEXT("Forgot to assign start up data to %s"), *GetName());
		// ensure(!CharacterStartUpData.IsNull());
#endif
	}

	InitStartUpData();
}

void ABlurCharacterBase::InitStartUpData()
{
	// 由具体子类实现。
}
