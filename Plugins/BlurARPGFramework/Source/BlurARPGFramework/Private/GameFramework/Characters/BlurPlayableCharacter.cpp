// Blur Feng All Rights Reserved.


#include "GameFramework/Characters/BlurPlayableCharacter.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Components/Combat/BlurAbilityCombatComponent.h"
#include "GameFramework/Components/Input/BlurEnhancedInputComponent.h"
#include "GameFramework/Components/UI/BlurCharacterUIComponent.h"
#include "GameFramework/DataAssets/Input/BlurDA_InputConfig.h"
#include "GameFramework/DataAssets/StartUp/BlurDA_CharacterStartup.h"
#include "GameFramework/GameModes/BlurGameModeBase.h"
#include "GameFramework/GameplayAbilitySystem/BlurAbilitySystemComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/BlurGameplayTags.h"
#include "GameFramework/Common/BlurDebugHelper.h"

ABlurPlayableCharacter::ABlurPlayableCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.f);

	// 关闭Rotation由控制器控制。
	bUseControllerRotationPitch = false; // 俯仰角。
	bUseControllerRotationYaw = false; // 偏航角。
	bUseControllerRotationRoll = false; // 翻滚角。

	// 创建相机臂。
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom")); //创建子对象，这个对象将包含在自身之下。
	CameraBoom->SetupAttachment(GetRootComponent()); //附着目标为角色自身的Root。
	CameraBoom->TargetArmLength = 200.f; //目标相机臂长度。
	CameraBoom->SocketOffset = FVector(0.f, 55.f, 65.f); //相机位置偏移，肩视角。
	CameraBoom->bUsePawnControlRotation = true; //使用Pawn的旋转作为相机旋转。

	// 创建相机。
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); //附着目标为相机臂。
	FollowCamera->bUsePawnControlRotation = true;

	// 设置角色移动组件。
	GetCharacterMovement()->bOrientRotationToMovement = true; // 旋转角色自身到加速度方向。
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f); // 旋转速率。
	GetCharacterMovement()->MaxWalkSpeed = 400.f; // 最大移动速度。
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f; // 刹车减速度。

	// 创建技能战斗组件。
	AbilityCombatComponent = CreateDefaultSubobject<UBlurAbilityCombatComponent>(TEXT("AbilityCombatComponent"));

	// 创建角色UI组件。
	CharacterUIComponent = CreateDefaultSubobject<UBlurCharacterUIComponent>(TEXT("CharacterUIComponent"));
}

UBlurCombatComponent* ABlurPlayableCharacter::GetPawnCombatComponent() const
{
	return AbilityCombatComponent;
}

UBlurPawnUIComponent* ABlurPlayableCharacter::GetPawnUIComponent() const
{
	return CharacterUIComponent;
}

UBlurCharacterUIComponent* ABlurPlayableCharacter::GetCharacterUIComponent() const
{
	return CharacterUIComponent;
}

void ABlurPlayableCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

void ABlurPlayableCharacter::InitStartUpData()
{
	Super::InitStartUpData();

	if(CharacterStartUpData.IsNull()) return;

	UBlurDA_CharacterStartup* LoadedData = CharacterStartUpData.LoadSynchronous();
	if (!LoadedData) return;

	// int32 AbilityApplyLevel = 1;
	//
	// // 根据游戏难度设置技能等级。
	// // Tips：游戏没有技能升级机制，所以此处将技能等级和游戏难度相关。
	// if (const ABlurGameModeBase* BaseGameMode = GetWorld()->GetAuthGameMode<ABlurGameModeBase>())
	// {
	// 	switch (BaseGameMode->GetCurrentGameDifficulty())
	// 	{
	// 	case EBlurGameDifficulty::Easy:
	// 		AbilityApplyLevel = 4;
	// 		break;
	// 	case EBlurGameDifficulty::Normal:
	// 		AbilityApplyLevel = 3;
	// 		break;
	// 	case EBlurGameDifficulty::Hard:
	// 		AbilityApplyLevel = 2;
	// 		break;
	// 	case EBlurGameDifficulty::VeryHard:
	// 		AbilityApplyLevel = 1;
	// 		break;
	// 	}
	// }
			
	LoadedData->GiveToAbilitySystemComponent(BlurAbilitySystemComponent);
}

void ABlurPlayableCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 确认InputConfigDataAsset资源是否配置，不为null。
#if WITH_EDITOR
	if (!InputConfigDataAsset)
	{
		Debug::Print("Forgot to assign a valid data asset as Input Config Data Asset.", FColor::Red);
		return;
	}
#else
	checkf(InputConfigDataAsset, TEXT("Forgot to assign a valid data asset as Input Config Data Asset."));
#endif
	
	// 调用父类方法，实际上里面没有具体的内容，但之后引擎更新可能会有实际实现的内容。
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 获取本地玩家控制器。
	const ULocalPlayer* LocalPlayer = GetController<APlayerController>()->GetLocalPlayer();

	// 通过增强输入子系统来添加配置的输入映射文件。
	// 在输入映射文件中，我们给每个InputAction配置可以出发的硬件输入，比如键盘或手柄的输入。
	UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
	check(Subsystem);
	// 设置默认输入映射。
	Subsystem->AddMappingContext(InputConfigDataAsset->DefaultMappingContext, 0);
	
#if WITH_EDITOR
	// Tips：在项目配置中设置默认的InputComponent为UBlurEnhancedInputComponent。在 ProjectSettings > Engine-Input > Default Classes - DefaultInputComponentClass 中设置。
	UBlurEnhancedInputComponent* BlurEnhancedInputComponent = Cast<UBlurEnhancedInputComponent>(PlayerInputComponent);
	if (!BlurEnhancedInputComponent)
	{
		Debug::Print("Please set the DefaultInputComponentClass of the project settings to BlurEnhancedInputComponent.", FColor::Red);
		return;
	}
#else
	UBlurEnhancedInputComponent* BlurEnhancedInputComponent = CastChecked<UBlurEnhancedInputComponent>(PlayerInputComponent);
#endif
	// 绑定输入事件。
	// 基础操作输入。如果子类自己在蓝图中实现，可以不配置这些输入事件。
	if (InputConfigDataAsset->NativeInputActions.Contains(BlurGameplayTags::Input_Move))
	{
		BlurEnhancedInputComponent->BindNativeInputAction(
			InputConfigDataAsset, BlurGameplayTags::Input_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move);
	}
	if (InputConfigDataAsset->NativeInputActions.Contains(BlurGameplayTags::Input_Look))
	{
		BlurEnhancedInputComponent->BindNativeInputAction(
			InputConfigDataAsset, BlurGameplayTags::Input_Look, ETriggerEvent::Triggered, this, &ThisClass::Input_Look);
	}
	
	// 切换目标输入。根据用途可以达到不同的效果。比如激活目标锁定技能后，切换目标输入。
	BlurEnhancedInputComponent->BindNativeInputAction(
		InputConfigDataAsset, BlurGameplayTags::Input_SwitchTarget, ETriggerEvent::Triggered, this, &ThisClass::Input_SwitchTargetTriggered);
	BlurEnhancedInputComponent->BindNativeInputAction(
		InputConfigDataAsset, BlurGameplayTags::Input_SwitchTarget, ETriggerEvent::Completed, this, &ThisClass::Input_SwitchTargetCompleted);

	// 重置玩家视角。
	BlurEnhancedInputComponent->BindNativeInputAction(
		InputConfigDataAsset, BlurGameplayTags::Input_ResetView, ETriggerEvent::Completed, this, &ThisClass::Input_SwitchTargetCompleted);

	// 拾取物品输入。
	BlurEnhancedInputComponent->BindNativeInputAction(
		InputConfigDataAsset, BlurGameplayTags::Input_PickUp, ETriggerEvent::Started, this,
		&ThisClass::Input_PickUpStonesStarted);
	
	// 技能输入。
	BlurEnhancedInputComponent->BindAbilityInputAction(
		InputConfigDataAsset, this, &ThisClass::Input_AbilityInputPressed,
		&ThisClass::Input_AbilityInputReleased, &ThisClass::Input_AbilityInputTriggered);
}

void ABlurPlayableCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ABlurPlayableCharacter::Input_Move(const FInputActionValue& InputActionValue)
{
	const FVector2D MovementVector = InputActionValue.Get<FVector2D>(); // 获取输入值。

	// 移动方向旋转数据，根据玩家控制器Yaw旋转构建。
	// 之后通过此Rotator构建向先后和左右移动的方向。实际上Control的朝向和Camera一致，我们就能根据Camera朝向进行移动。
	const FRotator MovementRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);

	// 前后移动。
	if(MovementVector.Y != 0.f)
	{
		const FVector ForwardDirection = MovementRotation.RotateVector(FVector::ForwardVector); // 构建移动方向。
		AddMovementInput(ForwardDirection, MovementVector.Y); // 按轴向进行移动。
	}

	// 左右移动。
	if(MovementVector.X != 0.f)
	{
		const FVector RightDirection = MovementRotation.RotateVector(FVector::RightVector);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ABlurPlayableCharacter::Input_Look(const FInputActionValue& InputActionValue)
{
	const FVector2D LookAxisVector = InputActionValue.Get<FVector2D>();

	// 左右摆动。
	if(LookAxisVector.X != 0.f)
	{
		AddControllerYawInput(LookAxisVector.X);
	}

	// 上下摆动。
	if(LookAxisVector.Y != 0.f)
	{
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ABlurPlayableCharacter::Input_SwitchTargetTriggered(const FInputActionValue& InputActionValue)
{
	FGameplayEventData Data;
	Data.EventMagnitude = InputActionValue.Get<FVector2D>().X;
	
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		this,
		BlurGameplayTags::Common_Event_SwitchTarget_Triggered,
		Data);
}

void ABlurPlayableCharacter::Input_SwitchTargetCompleted(const FInputActionValue& InputActionValue)
{
	FGameplayEventData Data;
	Data.EventMagnitude = InputActionValue.Get<FVector2D>().X;
	
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		this,
		BlurGameplayTags::Common_Event_SwitchTarget_Completed,
		Data);
}

void ABlurPlayableCharacter::Input_ResetView(const FInputActionValue& InputActionValue)
{
	const FGameplayEventData Data;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		this,
		BlurGameplayTags::Common_Event_ResetView,
		Data);
}

void ABlurPlayableCharacter::Input_PickUpStonesStarted(const FInputActionValue& InputActionValue)
{
	const FGameplayEventData Data;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(
		this,
		BlurGameplayTags::Common_Event_PickUp,
		Data);
}

// Tips：这里没有使用const和&关键字，因为此回调方法最终作为UEnhancedInputComponent->BindAction()方法的参数使用，而此类对回调方法的格式是这么要求的。
void ABlurPlayableCharacter::Input_AbilityInputPressed(FGameplayTag InInputTag)
{
	BlurAbilitySystemComponent->OnAbilityInputPressed(InInputTag);
}

void ABlurPlayableCharacter::Input_AbilityInputReleased(FGameplayTag InInputTag)
{
	BlurAbilitySystemComponent->OnAbilityInputReleased(InInputTag);
}

void ABlurPlayableCharacter::Input_AbilityInputTriggered(FGameplayTag InInputTag)
{
	BlurAbilitySystemComponent->OnAbilityInputTriggered(InInputTag);
}
