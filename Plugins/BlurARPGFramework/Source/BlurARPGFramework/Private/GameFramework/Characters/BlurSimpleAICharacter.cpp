// Blur Feng All Rights Reserved.


#include "GameFramework/Characters/BlurSimpleAICharacter.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Components/WidgetComponent.h"
#include "Engine/AssetManager.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Components/Combat/BlurAbilityCombatComponent.h"
#include "GameFramework/Components/UI/BlurCharacterUIComponent.h"
#include "GameFramework/Controllers/BlurAIController.h"
#include "GameFramework/DataAssets/StartUp/BlurDA_CharacterStartup.h"
#include "GameFramework/GameModes/BlurGameModeBase.h"
#include "GameFramework/GameplayAbilitySystem/BlurAbilitySystemComponent.h"
#include "GameFramework/UserWidgets/BlurWidgetBase.h"

ABlurSimpleAICharacter::ABlurSimpleAICharacter()
{
	// 生成时接入AI。
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// 设置角色移动组件。
	GetCharacterMovement()->bUseControllerDesiredRotation = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 180.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 1000.f;

	// 创建战斗组件。
	AbilityCombatComponent = CreateDefaultSubobject<UBlurAbilityCombatComponent>("AbilityCombatComponent");

	// 创建UI组件。
	CharacterUIComponent = CreateDefaultSubobject<UBlurCharacterUIComponent>("CharacterUIComponent");

	// 创建血条UI组件并附加到自身Mesh。
	HealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("HealthWidgetComponent");
	HealthWidgetComponent->SetupAttachment(GetMesh());
	
}

UBlurCombatComponent* ABlurSimpleAICharacter::GetPawnCombatComponent() const
{
	return AbilityCombatComponent;
}

UBlurPawnUIComponent* ABlurSimpleAICharacter::GetPawnUIComponent() const
{
	return CharacterUIComponent;
}

UBlurCharacterUIComponent* ABlurSimpleAICharacter::GetCharacterUIComponent() const
{
	return CharacterUIComponent;
}

void ABlurSimpleAICharacter::BeginPlay()
{
	Super::BeginPlay();

	// 获取自身AI控制器。
	WarriorAIController = Cast<ABlurAIController>(UAIBlueprintHelperLibrary::GetAIController(this));

	// 初始化血条UI组件。
	if (UBlurWidgetBase* HealthWidget = Cast<UBlurWidgetBase>(HealthWidgetComponent->GetUserWidgetObject()))
	{
		HealthWidget->InitWidget(this);
	}
}

void ABlurSimpleAICharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
}

void ABlurSimpleAICharacter::InitStartUpData()
{
	Super::InitStartUpData();

	if(CharacterStartUpData.IsNull()) return;

	int32 AbilityApplyLevel = 1;

	// // 根据游戏难度设置技能等级。
	// // Tips：游戏没有技能升级机制，所以此处将技能等级和游戏难度相关。
	// if (ABlurGameModeBase* BaseGameMode = GetWorld()->GetAuthGameMode<ABlurGameModeBase>())
	// {
	// 	switch (BaseGameMode->GetCurrentGameDifficulty())
	// 	{
	// 	case EBlurGameDifficulty::Easy:
	// 		AbilityApplyLevel = 1;
	// 		break;
	// 	case EBlurGameDifficulty::Normal:
	// 		AbilityApplyLevel = 2;
	// 		break;
	// 	case EBlurGameDifficulty::Hard:
	// 		AbilityApplyLevel = 3;
	// 		break;
	// 	case EBlurGameDifficulty::VeryHard:
	// 		AbilityApplyLevel = 4;
	// 		break;
	// 	}
	// }

	// 异步加载资产。简单AI角色通常数量众多且不是最主要的对象。我们使用异步加载所需资源，
	UAssetManager::GetStreamableManager().RequestAsyncLoad(
		CharacterStartUpData.ToSoftObjectPath(),
		FStreamableDelegate::CreateLambda(
			[this, AbilityApplyLevel]()
			{
				// 加载完成回调函数。
				if (UBlurDA_CharacterStartup* LoadedData = CharacterStartUpData.Get())
				{
					LoadedData->GiveToAbilitySystemComponent(BlurAbilitySystemComponent, AbilityApplyLevel);

					//Debug::Print(TEXT("Simple AI Character Start Up Data Loaded."), FColor::Green);
				}
			})
		);
}