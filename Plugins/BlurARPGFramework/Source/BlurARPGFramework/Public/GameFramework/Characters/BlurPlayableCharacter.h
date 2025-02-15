// Blur Feng All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Characters/BlurCharacterBase.h"
#include "GameFramework/Components/Combat/BlurAbilityCombatComponent.h"
#include "BlurPlayableCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
struct FInputActionValue;
struct FGameplayTag;
class UBlurDA_InputConfig;
class UBlurAbilityCombatComponent;

// 可游玩角色。一般用于制作玩家控制的角色。
UCLASS()
class BLURARPGFRAMEWORK_API ABlurPlayableCharacter : public ABlurCharacterBase
{
	GENERATED_BODY()

public:
	explicit ABlurPlayableCharacter(const FObjectInitializer& ObjectInitializer);
	
	//~ Begin IPawnUIInterface Interface.
	virtual UBlurPawnUIComponent* GetPawnUIComponent() const override;
	virtual UBlurCharacterUIComponent* GetCharacterUIComponent() const override;
	//~ End IPawnUIInterface Interface

protected:
	//~ Begin APawn Interface.
	virtual void PossessedBy(AController* NewController) override;
	//~ End APawn Interface

	virtual void InitStartUpData() override;
	
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;

#pragma region Components

	// Notes:
	// UPROPERTY宏用来标记UE类中的成员变量，告知引擎如何管理这些成员变量。
	// VisibleAnywhere：在UE编辑器的任何地方可见。只读。
	// BlueprintReadOnly：蓝图中只读。如果想要编辑使用BlueprintReadWrite。
	// Category：分类，有助于整理代码。蓝图中也会显示到相应分类下。
	// meta = (AllowPrivateAccess = "true")：允许蓝图或编辑器访问private成员变量。

	// UI组件。
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Blur ARPG Framework | UI", meta = (AllowPrivateAccess = "true"))
	UBlurCharacterUIComponent* CharacterUIComponent;
	
#pragma endregion

#pragma region Inputs
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Blur ARPG Framework | Character Data", meta = (AllowPrivateAccess = "true"))
	UBlurDA_InputConfig* InputConfigDataAsset;
	
	// 基础输入。
	void Input_Move(const FInputActionValue& InputActionValue);
	void Input_Look(const FInputActionValue& InputActionValue);
	// 切换目标输入。在锁定目标时。
	void Input_SwitchTargetTriggered(const FInputActionValue& InputActionValue);
	void Input_SwitchTargetCompleted(const FInputActionValue& InputActionValue);
	void Input_ResetView(const FInputActionValue& InputActionValue);

	void Input_PickUpStarted(const FInputActionValue& InputActionValue);
	
	// Tips：这里没有使用const和&关键字，因为此回调方法最终作为UEnhancedInputComponent->BindAction()方法的参数使用，而此类对回调方法的格式是这么要求的。
	// 技能输入。
	void Input_AbilityInputPressed(FGameplayTag InInputTag);
	void Input_AbilityInputReleased(FGameplayTag InInputTag);
	void Input_AbilityInputTriggered(FGameplayTag InInputTag);
	
#pragma endregion

public:
	FORCEINLINE UBlurAbilityCombatComponent* GetAbilityCombatComponent() const { return Cast<UBlurAbilityCombatComponent>(CombatComponent); }
};
