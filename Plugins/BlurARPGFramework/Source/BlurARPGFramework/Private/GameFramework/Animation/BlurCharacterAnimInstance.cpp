// Blur Feng All Rights Reserved.


#include "GameFramework/Animation/BlurCharacterAnimInstance.h"

#include "KismetAnimationLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Characters/BlurCharacterBase.h"
#include "Kismet/KismetMathLibrary.h"

void UBlurCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	OwningCharacter = Cast<ABlurCharacterBase>(TryGetPawnOwner());

	if(OwningCharacter)
	{
		OwningMovementComponent = OwningCharacter->GetCharacterMovement();
	}
}

void UBlurCharacterAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	if(!OwningCharacter || !OwningMovementComponent) return;
	
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	GroundSpeedRate = UKismetMathLibrary::SafeDivide(OwningCharacter->GetVelocity().Size2D(), OwningMovementComponent->MaxWalkSpeed);
	
	bHasAcceleration = OwningMovementComponent->GetCurrentAcceleration().SizeSquared2D() > 0.f;

	LocomotionDirection = UKismetAnimationLibrary::CalculateDirection(OwningCharacter->GetVelocity(), OwningCharacter->GetActorRotation());
}
