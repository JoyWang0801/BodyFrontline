// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Base.h"
#include "Characters/RBCCharacter.h"
#include "Items/Soul.h"
#include "Characters/WhiteBloodCellCharacter.h"
#include "Kismet/GameplayStatics.h"

ABase::ABase()
{
}

void ABase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator CurrentRotation = GetActorRotation();
	SetActorRotation(FRotator(CurrentRotation.Pitch, CurrentRotation.Yaw + 0.25, CurrentRotation.Roll));
}

void ABase::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("%s overlap."), *OverlappedComponent->GetName());


	ARBCCharacter* RBC = Cast<ARBCCharacter>(OtherActor);
	if (RBC)
	{
		//RBC->SetInBaseArea(true);
		ASoul* Holding = RBC->GetHoldingSoul();
		if(Holding)
		{
			Holding->DisableSphereCollision();
			AWhiteBloodCellCharacter* WBC = Cast<AWhiteBloodCellCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
			WBC->AddSouls(Holding);
			//DisableSphereCollision();
			SpawnPickupSystem();
			SpawnPickupSound();

			Holding->Destroy();
		}
	}
}
