// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/RBCAnimInstance.h"
#include "Characters/RBCCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void URBCAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	// get pawn owner
	ARBCCharacter* Character = Cast<ARBCCharacter>(TryGetPawnOwner());
	if (Character)
	{
		CharacterMovement = Character->GetCharacterMovement();
	}
}

void URBCAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);
	if (CharacterMovement)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(CharacterMovement->Velocity);
		IsFalling = CharacterMovement->IsFalling();
	}

}
