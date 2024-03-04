// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/WBCAnimInstance.h"
#include "Characters/WhiteBloodCellCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UWBCAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	// get pawn owner
	WBCCharacter = Cast<AWhiteBloodCellCharacter>(TryGetPawnOwner());
	if (WBCCharacter)
	{
		WBCCharacterMovement = WBCCharacter->GetCharacterMovement();
	}
}

void UWBCAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (WBCCharacterMovement) 
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(WBCCharacterMovement->Velocity);
		IsFalling = WBCCharacterMovement->IsFalling();
	}
}
