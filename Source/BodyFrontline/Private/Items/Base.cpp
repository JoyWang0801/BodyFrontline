// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Base.h"
#include "Characters/RBCCharacter.h"
#include "Items/Soul.h"

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
	ARBCCharacter* RBC = Cast<ARBCCharacter>(OtherActor);
	if (RBC)
	{
		ASoul* Holding = RBC->GetHoldingSoul();
		//DisableSphereCollision();
		SpawnPickupSystem();
		SpawnPickupSound();

		Holding->Destroy();
	}
}
