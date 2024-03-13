// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

class AWeapon;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BODYFRONTLINE_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCombatComponent();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	friend class AWhiteBloodCellCharacter;
	void EquipWeapon(AWeapon* WeaponToEquip);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	void FireButtonPressed(bool bIsPressed);
	void TraceToCrosshairs(FHitResult& TraceHitResult);
	FVector CrosshairPosition = FVector(0.f);
	bool isInEyeSight = false;
private:	
	class AWhiteBloodCellCharacter* Character;
	AWeapon* EquippedWeapon;
	FVector HitTarget;
	bool bFireButtonPressed;
		
};
