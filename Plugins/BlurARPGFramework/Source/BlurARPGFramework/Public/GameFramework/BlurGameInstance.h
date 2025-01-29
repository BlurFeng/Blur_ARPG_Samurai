// Blur Feng All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameplayTagContainer.h"

#include "BlurGameInstance.generated.h"

USTRUCT(BlueprintType)
struct FBlurGameLevelSet
{
	GENERATED_BODY()

	// 关卡Tag。
	UPROPERTY(EditDefaultsOnly, meta = (Categories = "GameData.Level"))
	FGameplayTag LevelTag;

	// 关卡。
	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> Level;

	bool IsValid() const
	{
		return LevelTag.IsValid() && !Level.IsNull();
	}
};

// 游戏实例。可用于全局数据管理。
// 提供了常用的需要配置的数据。
UCLASS()
class BLURARPGFRAMEWORK_API UBlurGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;

protected:
	// 加载地图前。
	virtual void OnPreloadMap(const FString& MapName);
	// 目标地图加载完成后。
	virtual void OnDestinationWorldLoaded(UWorld* LoadedWorld);

	// 游戏地图设置。
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FBlurGameLevelSet> GameLevelSets;

public:
	
	/// 获取游戏关卡，根据Tag。
	/// @param InTag 
	/// @return 
	UFUNCTION(BlueprintPure, meta = (Categories = "GameData.Level"))
	TSoftObjectPtr<UWorld> GetGameLevelByTag(const FGameplayTag InTag) const;
};
