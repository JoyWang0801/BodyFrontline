// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Base.h"
#include "Characters/RBCCharacter.h"
#include "Items/Soul.h"
#include "Characters/WhiteBloodCellCharacter.h"
#include "Kismet/GameplayStatics.h"

ABase::ABase()
{

	Tags.Add(FName("Base"));
}

void ABase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator CurrentRotation = GetActorRotation();
	SetActorRotation(FRotator(CurrentRotation.Pitch, CurrentRotation.Yaw + 0.25, CurrentRotation.Roll));
}

float ABase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Health = FMath::Clamp(Health - DamageAmount, 0.f, MaxHealth);
	//UE_LOG(LogTemp, Warning, TEXT("Base health: %f."), GetHealthPercent());
	 
	return 0.0f;
}

void ABase::UpdateHealth()
{
	UE_LOG(LogTemp, Warning, TEXT("Base health++"));

	Health = FMath::Clamp(Health + 1.f, 0.f, MaxHealth);
}

void ABase::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag(FName("RBC")))
	{
		ARBCCharacter* RBC = Cast<ARBCCharacter>(OtherActor);
		if (RBC)
		{
			ASoul* Holding = RBC->GetHoldingSoul();
			if (Holding)
			{
				Holding->DisableSphereCollision();
				AWhiteBloodCellCharacter* WBC = Cast<AWhiteBloodCellCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
				WBC->AddSouls(Holding);
				SpawnPickupSystem();
				SpawnPickupSound();

				Holding->Destroy();
			}
		}
	}
}

void ABase::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(HealthTimer, this, &ABase::UpdateHealth, 1.0f, true);
}

float ABase::GetHealthPercent()
{
	return Health / MaxHealth;
}
