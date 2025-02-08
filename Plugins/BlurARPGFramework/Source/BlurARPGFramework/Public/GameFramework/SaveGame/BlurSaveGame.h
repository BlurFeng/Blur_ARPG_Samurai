// Blur Feng All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GameFramework/Types/BlurEnumTypes.h"
#include "BlurSaveGame.generated.h"

// 存储游戏数据。
UCLASS()
class BLURARPGFRAMEWORK_API UBlurSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	// 游戏难度。
	UPROPERTY(BlueprintReadOnly)
	EBlurGameDifficulty SavedGameDifficulty = EBlurGameDifficulty::Normal;
};
