// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/HealPack.h"
#include "Characters/WhiteBloodCellCharacter.h"

void AHealPack::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag(FName("WBC")))
	{
		AWhiteBloodCellCharacter* Character = Cast<AWhiteBloodCellCharacter>(OtherActor);
		if (Character)
		{
			Character->UseItem(Type);
		}

		Destroy();
	}
}
