// Blur Feng All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Interfaces/BlurPawnCombatInterface.h"
#include "GameFramework/Interfaces/BlurPawnUIInterface.h"

#include "BlurCharacterBase.generated.h"

struct FGameplayTagContainer;
// 角色基础类。
UCLASS()
class BLURARPGFRAMEWORK_API ABlurCharacterBase : public ACharacter, public IAbilitySystemInterface, public IBlurPawnCombatInterface, public IBlurPawnUIInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABlurCharacterBase();
	
	// Notes：IAbilitySystemInterface 接口是GAS和Character通信的重要接口。
	// GAS通过此接口和Character通信。此接口在GAS系统的各种地方使用到。

	//~ Begin IAbilitySystemInterface Interface.
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	//~ End IAbilitySystemInterface Interface

	//~ Begin IPawnCombatInterface Interface.
	virtual UBlurCombatComponent* GetPawnCombatComponent() const override;
	//~ End IPawnCombatInterface Interface

	//~ Begin IPawnUIInterface Interface.
	virtual UBlurPawnUIComponent* GetPawnUIComponent() const override;
	//~ End IPawnUIInterface Interface
	
protected:
	//~ Begin APawn Interface.
	virtual void PossessedBy(AController* NewController) override;
	//~ End APawn Interface

	virtual void InitStartUpData();

	// 技能系统组件（属于GAS插件的一部分）。
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "Blur ARPG Framework | Ability System")
	class UBlurAbilitySystemComponent* BlurAbilitySystemComponent;

	// 属性集（属于GAS插件的一部分）。
	 UPROPERTY(VisibleAnywhere, BlueprintReadOnly, category = "Blur ARPG Framework | Ability System")
	 class UBlurAttributeSet* BlurAttributeSet;

	// 角色启动数据资源。
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Blur ARPG Framework | Character Data")
	TSoftObjectPtr<class UBlurDA_CharacterStartup> CharacterStartUpData;

public:

	// Notes：FORCEINLINE宏用于强制内联化函数。
	// 非常适合用于访问器方法这样频繁调用且短小的方法，以提高性能。
	// 它将函数的实现直接嵌入调用点，而不是生成函数调用指令，通过栈跳转来调用函数。
	// 但需要注意，这会导致代码膨胀，不适合用于较大的函数。否则会增加缓存压力，适得其反。
	
	FORCEINLINE UBlurAbilitySystemComponent* GetBlurAbilitySystemComponent() const { return BlurAbilitySystemComponent; }
	FORCEINLINE UBlurAttributeSet* GetBlurAttributeSet() const { return BlurAttributeSet; }

	// 属性集相关方法。用于快速获取属性集的内容。
#pragma region Attribute Set
	UFUNCTION(BlueprintPure, Category = "Blur ARPG Framework | Ability System | Attribute Set")
	float GetMaxHealth() const;
	
	UFUNCTION(BlueprintPure, Category = "Blur ARPG Framework | Ability System | Attribute Set")
	float GetHealth() const;
	
	UFUNCTION(BlueprintPure, Category = "Blur ARPG Framework | Ability System | Attribute Set")
	float GetMaxStamina() const;
	
	UFUNCTION(BlueprintPure, Category = "Blur ARPG Framework | Ability System | Attribute Set")
	float GetStamina() const;
	
	UFUNCTION(BlueprintPure, Category = "Blur ARPG Framework | Ability System | Attribute Set")
	float GetMaxRage() const;
	
	UFUNCTION(BlueprintPure, Category = "Blur ARPG Framework | Ability System | Attribute Set")
	float GetRage() const;

	UFUNCTION(BlueprintPure, Category = "Blur ARPG Framework | Ability System | Attribute Set")
	float GetMaxExperienceValue() const;
	
	UFUNCTION(BlueprintPure, Category = "Blur ARPG Framework | Ability System | Attribute Set")
	float GetExperienceValue() const;
	
	UFUNCTION(BlueprintPure, Category = "Blur ARPG Framework | Ability System | Attribute Set")
	float GetAttackPower() const;
	
	UFUNCTION(BlueprintPure, Category = "Blur ARPG Framework | Ability System | Attribute Set")
	float GetDefensePower() const;
#pragma endregion

	/// 尝试通过 Tag 激活技能。
	/// @param AbilityTags 
	/// @param AllowRemoteActivation 
	/// @return 
	UFUNCTION(BlueprintCallable, Category = "Blur ARPG Framework | Ability System")
	bool ActivateAbilitiesByTag(const FGameplayTagContainer AbilityTags, const bool AllowRemoteActivation);
};
