// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/RBCCharacter.h"
#include "HUD/HealthBarComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Items/Soul.h"


// Sets default values
ARBCCharacter::ARBCCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBar"));
	HealthBarWidget->SetupAttachment(GetRootComponent());

	this->Tags.Add(FName("RBC"));
}

void ARBCCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ARBCCharacter::ReceiveDamage(float Damage)
{
	Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);
}

float ARBCCharacter::GetHealthPercent()
{
	return Health / MaxHealth;
}

void ARBCCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float ARBCCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (HealthBarWidget)
	{
		ReceiveDamage(DamageAmount);
		HealthBarWidget->SetHealthPercent(GetHealthPercent());
	}

	if (Health <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("RBCDie"));
		RBCDie();
	}
	return DamageAmount;
}

void ARBCCharacter::HoldSoul(ASoul* SoulToHold)
{
	if (SoulToHold == nullptr) return;

	HoldedSoul = SoulToHold;
	// HoldedSoul->SetWeaponState(EWeaponState::EWS_Equipped);
	const USkeletalMeshSocket* HandSocket = GetMesh()->GetSocketByName(FName("RightHandSoulSocket"));
	if (HandSocket)
	{
		HandSocket->AttachActor(HoldedSoul, GetMesh());
	}
	HoldedSoul->SetOwner(this);
	HoldedSoul->SetInstigator(this);
	HoldedSoul->DisableSphereCollision();
}

