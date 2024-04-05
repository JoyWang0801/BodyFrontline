// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Interfaces/GameEnums.h"
#include "HealPack.generated.h"

/**
 * 
 */
UCLASS()
class BODYFRONTLINE_API AHealPack : public AItem
{
	GENERATED_BODY()

public:
	FORCEINLINE EItemType GetType() { return Type; }
	FORCEINLINE int32 GetAmount() { return Amount; }

protected:
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

private:
	EItemType Type = EItemType::EIT_HealPack;
	int32 Amount = 35;
};