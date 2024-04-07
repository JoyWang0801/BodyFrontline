// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Base.generated.h"

#define OXYGEN_WEIGHT 5

UCLASS()
class BODYFRONTLINE_API ABase : public AItem
{
	GENERATED_BODY()

public:
	ABase();
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	void UpdateHealthBar();
	void UpdateHealth();

protected:
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	class UHealthBarComponent* HealthBarWidget;

	float GetHealthPercent();

	/* Combat and Attribute*/
	UPROPERTY(EditAnywhere, Category = "Actor stats")
	float MaxHealth = 100.f;

	// Current health
	UPROPERTY(EditAnywhere, Category = "Actor stats")
	float Health = 30.f;

	FTimerHandle HealthTimer;
	int32 SoulCount = 0;
};
