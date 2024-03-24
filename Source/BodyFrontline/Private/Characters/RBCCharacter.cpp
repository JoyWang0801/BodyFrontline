// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/RBCCharacter.h"
#include "HUD/HealthBarComponent.h"


// Sets default values
ARBCCharacter::ARBCCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBar"));
	HealthBarWidget->SetupAttachment(GetRootComponent());

	Tags.Add(FName("RBC"));
}

// Called when the game starts or when spawned
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

