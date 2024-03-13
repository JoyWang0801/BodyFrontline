// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/CombatComponent.h"
#include "Weapon/Weapon.h"
#include "Characters/WhiteBloodCellCharacter.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UCombatComponent::UCombatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();	
}

void UCombatComponent::FireButtonPressed(bool bIsPressed)
{
	bFireButtonPressed = bIsPressed;

	if (EquippedWeapon == nullptr) return;
	if (Character && bIsPressed && isInEyeSight)
	{
		Character->PlayFireMontage();
		EquippedWeapon->Fire(HitTarget);
	}
}

void UCombatComponent::TraceToCrosshairs(FHitResult& HitResult)
{
	FVector Head = Character->GetMesh()->GetSocketLocation("Head");
	GetWorld()->LineTraceSingleByChannel(
		HitResult,
		Head,
		CrosshairPosition,
		ECollisionChannel::ECC_Visibility
	);

	// DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.f, 5, FColor::Purple, true, 5.f);

	if (!HitResult.bBlockingHit)
	{
		HitResult.ImpactPoint = CrosshairPosition;
		HitTarget = CrosshairPosition;
	}
	else 
	{
		HitTarget = HitResult.ImpactPoint;
	}


}


// Called every frame
void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FHitResult HitResult;
	TraceToCrosshairs(HitResult);
}

void UCombatComponent::EquipWeapon(AWeapon* WeaponToEquip)
{

	if (Character == nullptr || WeaponToEquip == nullptr) return;

	EquippedWeapon = WeaponToEquip;
	EquippedWeapon->SetWeaponState(EWeaponState::EWS_Equipped);
	const USkeletalMeshSocket* HandSocket = Character->GetMesh()->GetSocketByName(FName("RightHandSocket"));
	if (HandSocket) 
	{
		HandSocket->AttachActor(EquippedWeapon, Character->GetMesh());
	}
	EquippedWeapon->SetOwner(Character);
	EquippedWeapon->ShowPickupWidget(false);
	EquippedWeapon->SetInstigator(Character);
	EquippedWeapon->DisableSphereCollision();
}

