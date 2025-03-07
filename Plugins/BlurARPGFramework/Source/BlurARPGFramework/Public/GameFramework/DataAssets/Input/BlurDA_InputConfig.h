// Blur Feng All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayTagContainer.h"

#include "BlurDA_InputConfig.generated.h"

class UInputAction;
class UInputMappingContext;

// 输入事件配置结构体。
USTRUCT(BlueprintType)
struct FInputActionConfig
{
	GENERATED_BODY() // UE生成相关宏，否则无法生成。

public:

	// Notes：通过 meta = (Categories = "Input")，我们限定可选的Tag只能是Input.之下的Tag。
	
	// 输入行为对应的Tag。
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (Categories = "Input"))
	FGameplayTag InputTag;

	// 输入事件类。
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputAction* InputAction;

	bool IsValid() const
	{
		return InputTag.IsValid() && InputAction;
	}

	bool operator==(const FGameplayTag& InInputTag) const
	{
		return InInputTag == InputTag;
	}
};

// 输入配置资源。
// 我们使用UE5新的Enhanced Input增强输入系统来进行输入操作。
// 此DataAsset主要将GameplayTag和输入相关联，方便用于GAS技能系统。
UCLASS()
class BLURARPGFRAMEWORK_API UBlurDA_InputConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	
	// 默认输入映射。
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputMappingContext* DefaultMappingContext;

	// Notes:
	// UPROPERTY宏用来标记UE类中的成员变量，告知引擎如何管理这些成员变量。
	// EditDefaultsOnly：允许变量在蓝图类默认值面板中编辑，但运行时或实例化后的对象不可修改。
	// meta = (TitleProperty = "InputTag")：标头显示属性为InputTag。这会将蓝图编辑器中的抬头显示为 FInputActionConfig 中 InputTag 成员变量的信息，方便编辑。

	// 本地输入配置。
	// 移动，转动视角等行为。如果项目自己在蓝图中实现相关内容，可不配置此Tag。
	// 可用的相关配置：
	// Input_SwitchTarget: 切换目标。IA需要获取Axis2D鼠标移动输入.
	// Input_ResetView: 重置视口。
	// Input_PickUp: 拾取物品。
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputTag"))
	TArray<FInputActionConfig> NativeInputActions;

	// Notes；方法最后的const关键字保证此方法是只读方法，不修改任何变量。

	/// 通过InputTag查找对应的InputAction。
	/// @param InInputTag
	/// @return 
	UInputAction* FindNativeInputActionByTag(const FGameplayTag& InInputTag) const;

	// 技能输入映射。
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputTag"))
	TArray<FInputActionConfig> AbilityInputActions;
};

// 绑定技能输入数据资源。
// 可用于武器类的配置。在武器装备到角色时，绑定相应的技能输入配置。
UCLASS()
class BLURARPGFRAMEWORK_API UBlurDA_InputConfigWithAbilities : public UDataAsset
{
	GENERATED_BODY()

public:
	
	// 输入映射。
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UInputMappingContext* InputMappingContext;

	// 输入优先级。
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int InputMappingContextPriority = 1;
	
	// 输入事件配置。
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (TitleProperty = "InputTag"))
	TArray<FInputActionConfig> AbilityInputActions;
};