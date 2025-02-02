// Blur Feng All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Components/BlurPawnComponentBase.h"
#include "BlurPawnUIComponent.generated.h"

class UBlurWidgetBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPercentChangedDelegate, float, NewPercent);

// Pawn的UI组件。当Pawn比较简单，只用显示有限的信息时，可以使用此组件。比如简单的敌人。
UCLASS()
class BLURARPGFRAMEWORK_API UBlurPawnUIComponent : public UBlurPawnComponentBase
{
	GENERATED_BODY()

public:

	// Notes: BlueprintAssignable 蓝图可分配。
	// BlueprintAssignable 使得蓝图可以分配和使用此委托。因为 UI 的通信收发大多数在蓝图中操作。

	// Tips：这里提供了一个当前血量变化时的通知委托。
	// 根据项目的需求，你可以继承此类然后提供更多的通知委托用于更新UI。
	
	// 当前血量变化时。
	UPROPERTY(BlueprintAssignable)
	FOnPercentChangedDelegate OnHealthChanged;

	/// 注册Widget到UI组件。
	/// @param InWidgetToRegister 
	UFUNCTION(BlueprintCallable)
	void RegisterDrawnWidget(UBlurWidgetBase* InWidgetToRegister);

	/// 移除所有绘制的UI。
	UFUNCTION(BlueprintCallable)
	void RemoveDrawnWidgetsAll();

private:
	TArray<UBlurWidgetBase*> DrawnWidgets;
};
