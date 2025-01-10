// Blur Feng All Rights Reserved.


#include "GameFramework/BlurActorBase.h"

// Sets default values
ABlurActorBase::ABlurActorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABlurActorBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABlurActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

