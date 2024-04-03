// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Soul.h"
#include "Interfaces/PickupInterface.h"
#include "Characters/RBCCharacter.h"

ASoul::ASoul()
{
	// Super::AActor();

	Tags.Add(FName("Soul"));
}

void ASoul::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ARBCCharacter* RBC = Cast<ARBCCharacter>(OtherActor);
	if (RBC)
	{
		RBC->HoldSoul(this);
		DisableSphereCollision();
	}

	// WBC or Enemy, will change to enemy only
	//IPickupInterface* PickupInterface = Cast<IPickupInterface>(OtherActor);
	//if (PickupInterface)
	//{
	//	PickupInterface->AddSouls(this);

	//	SpawnPickupSystem();
	//	SpawnPickupSound();

	//	Destroy();
	//}
	//else // RBC
	//{
	//	ARBCCharacter* RBC = Cast<ARBCCharacter>(OtherActor);
	//	if (RBC)
	//	{
	//		RBC->HoldSoul(this);
	//		DisableSphereCollision();
	//		//SpawnPickupSystem();
	//		//SpawnPickupSound();

	//		//Destroy();
	//	}
	//}


}
