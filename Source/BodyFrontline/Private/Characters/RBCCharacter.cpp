// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/RBCCharacter.h"
#include "HUD/HealthBarComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "Items/Soul.h"
#include "Enemy/Enemy.h"


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

	if (HealthBarWidget)
	{
		HealthBarWidget->SetVisibility(false);
	}

	GetMesh()->bRenderCustomDepth = true;
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
		HealthBarWidget->SetVisibility(true);
		ReceiveDamage(DamageAmount);
		HealthBarWidget->SetHealthPercent(GetHealthPercent());

		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
			{
				HealthBarWidget->SetVisibility(false);

			}, 6, false);
	}

	if (Health <= 0)
	{
		//AEnemy* DmgCauser = Cast<AEnemy>(DamageCauser->GetOwner());
		//if (DmgCauser)
		//{
		//	// UE_LOG(LogTemp, Warning, TEXT("dmg causer: %s."), *DmgCauser->GetName());
		//	DmgCauser->IncreaseExp(5.f);
		//}
		RBCDie();
	}
	return DamageAmount;
}

void ARBCCharacter::DeliverSoul()
{
	if (IsHolding) 
	{
		HoldedSoul->Destroy();
		HoldedSoul = nullptr;
		IsHolding = false;
	}
}

void ARBCCharacter::HoldSoul(ASoul* SoulToHold)
{
	if (SoulToHold == nullptr) return;

	if (!IsHolding)
	{
		UE_LOG(LogTemp, Warning, TEXT("RBC Pick Up a soul"));

		HoldedSoul = SoulToHold;
		const USkeletalMeshSocket* HandSocket = GetMesh()->GetSocketByName(FName("RightHandSoulSocket"));
		if (HandSocket)
		{
			HandSocket->AttachActor(HoldedSoul, GetMesh());
		}

		IsHolding = true;
		HoldedSoul->SetOwner(this);
		HoldedSoul->SetInstigator(this);
		HoldedSoul->DisableSphereCollision();
	}
}

