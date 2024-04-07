// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RBCCharacter.generated.h"

UCLASS()
class BODYFRONTLINE_API ARBCCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ARBCCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	void DeliverSoul();
	void HoldSoul(class ASoul* SoulToHold);
	FORCEINLINE ASoul* GetHoldingSoul() { return HoldedSoul; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool GetIsHoldingSoul() { return IsHolding; }
	FORCEINLINE void SetIsHoldingSoul(bool b) { IsHolding = b; }

	UFUNCTION(BlueprintImplementableEvent, Category = "Death")
	void RBCDie();

protected:
	virtual void BeginPlay() override;

private:	
	void ReceiveDamage(float Damage);
	float GetHealthPercent();

	/* Widget */
	UPROPERTY(VisibleAnywhere)
	class UHealthBarComponent* HealthBarWidget;

	UPROPERTY(VisibleInstanceOnly)
	class AItem* OverlappingItem;

	/* Combat and Attribute*/
	UPROPERTY(EditAnywhere, Category = "Actor stats")
	float MaxHealth = 100.f;

	// Current health
	UPROPERTY(EditAnywhere, Category = "Actor stats")
	float Health = 100.f;

	ASoul* HoldedSoul;
	bool IsHolding = false;
};
