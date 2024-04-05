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
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	void UpdateHealth();

protected:
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	virtual void BeginPlay() override;

private:
	float GetHealthPercent();

	/* Combat and Attribute*/
	UPROPERTY(EditAnywhere, Category = "Actor stats")
	float MaxHealth = 30.f;

	// Current health
	UPROPERTY(EditAnywhere, Category = "Actor stats")
	float Health = 100.f;

	FTimerHandle HealthTimer;
};
