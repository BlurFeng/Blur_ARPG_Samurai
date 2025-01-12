// Blur Feng All Rights Reserved.


#include "GameFramework/UserWidgets/BlurWidgetBase.h"

#include "GameFramework/Interfaces/BlurPawnUIInterface.h"

void UBlurWidgetBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UBlurWidgetBase::InitWidget(AActor* OwningActor)
{
	if (const IBlurPawnUIInterface* PawnUIInterface = Cast<IBlurPawnUIInterface>(OwningActor))
	{
		UBlurPawnUIComponent* GotPawnUIComponent = PawnUIInterface->GetPawnUIComponent();

		checkf(GotPawnUIComponent, TEXT("Failed to extract an PawnUIComponent from %s"), *OwningActor->GetActorNameOrLabel());

		PawnUIComponent = TWeakObjectPtr<UBlurPawnUIComponent>(GotPawnUIComponent);

		if (UBlurCharacterUIComponent* GotCharacterUIComponent = Cast<UBlurCharacterUIComponent>(GotPawnUIComponent))
		{
			CharacterUIComponent = TWeakObjectPtr<UBlurCharacterUIComponent>(GotCharacterUIComponent);

			BP_OnInitializedViaCharacterUIComponent(GotCharacterUIComponent);
		}
		else
		{
			BP_OnInitialized(GotPawnUIComponent);
		}
	}
}

UBlurPawnUIComponent* UBlurWidgetBase::GetPawnUIComponent(EBlurValidType& OutValidType)
{
	if (PawnUIComponent.IsValid())
	{
		OutValidType = EBlurValidType::Valid;
		return PawnUIComponent.Get();
	}
	
	OutValidType = EBlurValidType::Invalid;
	return nullptr;
}

UBlurPawnUIComponent* UBlurWidgetBase::GetCharacterUIComponent(EBlurValidType& OutValidType)
{
	if (CharacterUIComponent.IsValid())
	{
		OutValidType = EBlurValidType::Valid;
		return CharacterUIComponent.Get();
	}
	
	OutValidType = EBlurValidType::Invalid;
	return nullptr;
}
