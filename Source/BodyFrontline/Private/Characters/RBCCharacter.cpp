// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/RBCCharacter.h"
#include "HUD/HealthBarComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Items/Soul.h"


// Sets default values
ARBCCharacter::ARBCCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBar"));
	HealthBarWidget->SetupAttachment(GetRootComponent());

	this->Tags.Add(FName("RBC"));

}

// Called when the game starts or when spawned
void ARBCCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (this->ActorHasTag(FName("RBC")))
	{
		// Tag exists on this object
		UE_LOG(LogTemp, Warning, TEXT("Tag 'RBC' successfully added."));
	}
	else
	{
		// Tag does not exist on this object
		UE_LOG(LogTemp, Warning, TEXT("Tag 'RBC' not found."));
	}
}

void ARBCCharacter::ReceiveDamage(float Damage)
{
	Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);
}

float ARBCCharacter::GetHealthPercent()
{
	return Health / MaxHealth;
}

// Called every frame
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

