#include "AuraAnimInstance.h"
#include "AuraCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "KismetAnimationLibrary.h"

void UAuraAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	AuraCharacter = Cast<AAuraCharacterBase>(TryGetPawnOwner());
	if (AuraCharacter)
	{
		AuraCharacterMovement = AuraCharacter->GetCharacterMovement();
	}
}

void UAuraAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (AuraCharacter == nullptr)
	{
		AuraCharacter = Cast<AAuraCharacterBase>(TryGetPawnOwner());
	}

	if (AuraCharacter == nullptr || AuraCharacterMovement == nullptr) return;

	FVector Velocity = AuraCharacter->GetVelocity();
	Velocity.Z = 0.f;
	GroundSpeed = Velocity.Size();
	
	// Calcular dirección (-180 a 180) relativo hacia dónde está viendo el personaje
	FRotator AimRotation = AuraCharacter->GetActorRotation();
	Direction = UKismetAnimationLibrary::CalculateDirection(Velocity, AimRotation);

	bShouldMove = (GroundSpeed > 3.f && AuraCharacterMovement->GetCurrentAcceleration() != FVector::ZeroVector);
	bIsFalling = AuraCharacterMovement->IsFalling();
}
