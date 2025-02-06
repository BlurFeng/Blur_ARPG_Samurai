// Blur Feng All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/Components/UI/BlurCharacterUIComponent.h"
#include "GameFramework/Components/UI/BlurPawnUIComponent.h"
#include "GameFramework/Types/BlurEnumTypes.h"
#include "BlurWidgetBase.generated.h"

// 用户UI组件基类。
UCLASS()
class BLURARPGFRAMEWORK_API UBlurWidgetBase : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;

	// Notes：BlueprintImplementableEvent 蓝图事件
	// BlueprintImplementableEvent 用于为蓝图提供事件。可以让C++通知一些内容给蓝图。
	// C++ 不用实现此方法。

	// 当Widget初始化时。
	UFUNCTION(BlueprintImplementableEvent, Category = "Blur ARPG Framework | Widget", meta = (DisplayName = "On Initialized"))
	void BP_OnInitialized(UBlurPawnUIComponent* OwningPawnUIComponent);

	// 当Widget初始化时。
	UFUNCTION(BlueprintImplementableEvent, Category = "Blur ARPG Framework | Widget", meta = (DisplayName = "On Initialized Character UI Component"))
	void BP_OnInitializedCharacterUIComponent(UBlurCharacterUIComponent* OwningCharacterUIComponent);
	
public:
	
	void InitWidget(AActor* OwningActor);

	UFUNCTION(BlueprintPure, Category = "Blur ARPG Framework | Widget", meta = (ExpandEnumAsExecs = "OutValidType"))
	UBlurPawnUIComponent* GetPawnUIComponent(EBlurValidType& OutValidType) const;

	UFUNCTION(BlueprintPure, Category = "Blur ARPG Framework | Widget", meta = (ExpandEnumAsExecs = "OutValidType"))
	UBlurPawnUIComponent* GetCharacterUIComponent(EBlurValidType& OutValidType) const;
	
private:
	UPROPERTY()
	TWeakObjectPtr<UBlurPawnUIComponent> PawnUIComponent;

	UPROPERTY()
	TWeakObjectPtr<UBlurCharacterUIComponent> CharacterUIComponent;
};
