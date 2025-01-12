// Blur Feng All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/Types/BlurEnumTypes.h"
#include "BlurGameModeBase.generated.h"

// 游戏模式基类。
UCLASS()
class BLURARPGFRAMEWORK_API ABlurGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ABlurGameModeBase();

protected:

	// 当前游戏难度。
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Game Settings")
	EBlurGameDifficulty CurrentGameDifficulty;

public:
	FORCEINLINE EBlurGameDifficulty GetCurrentGameDifficulty() const {return CurrentGameDifficulty;}
};
