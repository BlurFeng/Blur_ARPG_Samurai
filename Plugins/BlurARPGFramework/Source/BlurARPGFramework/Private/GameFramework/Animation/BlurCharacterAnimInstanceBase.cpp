// Blur Feng All Rights Reserved.


#include "GameFramework/Animation/BlurCharacterAnimInstanceBase.h"

#include "KismetAnimationLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Characters/BlurCharacterBase.h"
#include "Kismet/KismetMathLibrary.h"

void UBlurCharacterAnimInstanceBase::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	OwningCharacter = Cast<ABlurCharacterBase>(TryGetPawnOwner());

	if(OwningCharacter)
	{
		OwningMovementComponent = OwningCharacter->GetCharacterMovement();
	}
}

void UBlurCharacterAnimInstanceBase::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	if(!OwningCharacter || !OwningMovementComponent) return;
	
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	GroundSpeedRate = UKismetMathLibrary::SafeDivide(OwningCharacter->GetVelocity().Size2D(), OwningMovementComponent->MaxWalkSpeed);
	
	bHasAcceleration = OwningMovementComponent->GetCurrentAcceleration().SizeSquared2D() > 0.f;

	LocomotionDirection = UKismetAnimationLibrary::CalculateDirection(OwningCharacter->GetVelocity(), OwningCharacter->GetActorRotation());
}
