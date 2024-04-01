// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Base.generated.h"

/**
 * 
 */
UCLASS()
class BODYFRONTLINE_API ABase : public AItem
{
	GENERATED_BODY()

public:
	ABase();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
