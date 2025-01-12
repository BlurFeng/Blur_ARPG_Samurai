// Blur Feng All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Characters/BlurCharacterBase.h"
#include "BlurSimpleAICharacter.generated.h"

class UWidgetComponent;
class UBoxComponent;
class ABlurAIController;
class UBlurAbilityCombatComponent;

// 简单AI角色。一般用于制作敌人小兵。
// TODO：如果需要制作可以被玩家操控的角色，创建此类的子类并提供相关功能。（比如每个Character的相机组件，或者提供独立相机的可切换方法。）
UCLASS()
class BLURARPGFRAMEWORK_API ABlurSimpleAICharacter : public ABlurCharacterBase
{
	GENERATED_BODY()

public:
	ABlurSimpleAICharacter();

	//~ Begin IPawnCombatInterface Interface.
	virtual UBlurCombatComponentBase* GetPawnCombatComponent() const override;
	//~ End IPawnCombatInterface Interface

	//~ Begin IPawnUIInterface Interface.
	virtual UBlurPawnUIComponent* GetPawnUIComponent() const override;
	virtual UBlurCharacterUIComponent* GetCharacterUIComponent() const override;
	//~ End IPawnUIInterface Interface

protected:
	virtual void BeginPlay() override;
	
	//~ Begin APawn Interface.
	virtual void PossessedBy(AController* NewController) override;
	//~ End APawn Interface

	virtual void InitStartUpData() override;
	
	// 战斗组件。
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
	UBlurAbilityCombatComponent* AbilityCombatComponent;
	
	// 角色UI组件。
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UBlurCharacterUIComponent* CharacterUIComponent;

	// 血条UI组件。之后可以再蓝图配置具体生生的Widget类。
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* HealthWidgetComponent;

	// AI控制器。
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "UI")
	ABlurAIController* WarriorAIController;

public:
	FORCEINLINE UBlurAbilityCombatComponent* GetAbilityCombatComponent() const {return AbilityCombatComponent;}
};
