// Blur Feng All Rights Reserved.


#include "GameFramework/Components/BlurPawnComponentBase.h"

// Sets default values for this component's properties
UBlurPawnComponentBase::UBlurPawnComponentBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

APawn* UBlurPawnComponentBase::GetOwningPawn() const
{
	return GetOwningPawn<APawn>();
}

AController* UBlurPawnComponentBase::GetOwningController() const
{
	return GetOwningController<AController>();
}
