// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/WhiteBloodCellCharacter.h"
#include "Items/DamageBoost.h"

void ADamageBoost::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Collecting damage booster"));
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
