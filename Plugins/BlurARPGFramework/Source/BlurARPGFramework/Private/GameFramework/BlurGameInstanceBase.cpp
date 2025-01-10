// Blur Feng All Rights Reserved.


#include "GameFramework/BlurGameInstanceBase.h"

#include "MoviePlayer.h"

void UBlurGameInstanceBase::Init()
{
	Super::Init();

	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &ThisClass::OnPreloadMap);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &ThisClass::OnDestinationWorldLoaded);
}

void UBlurGameInstanceBase::OnPreloadMap(const FString& MapName)
{
	FLoadingScreenAttributes LoadingScreenAttributes;
	LoadingScreenAttributes.bAutoCompleteWhenLoadingCompletes = true;
	LoadingScreenAttributes.MinimumLoadingScreenDisplayTime = 2.f;
	// TODO：这里我们直接使用 NewTestLoadingScreenWidget 作为Load动画。
	LoadingScreenAttributes.WidgetLoadingScreen = FLoadingScreenAttributes::NewTestLoadingScreenWidget();

	GetMoviePlayer()->SetupLoadingScreen(LoadingScreenAttributes);
}

void UBlurGameInstanceBase::OnDestinationWorldLoaded(UWorld* LoadedWorld)
{
	GetMoviePlayer()->StopMovie();
}

TSoftObjectPtr<UWorld> UBlurGameInstanceBase::GetGameLevelByTag(const FGameplayTag InTag) const
{
	for (const FBlurGameLevelSet& GameLevelSet : GameLevelSets)
	{
		if (!GameLevelSet.IsValid()) continue;

		if (GameLevelSet.LevelTag == InTag)
		{
			return GameLevelSet.Level;
		}
	}

	return TSoftObjectPtr<UWorld>();
}