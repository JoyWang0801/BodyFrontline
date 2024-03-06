// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "WhiteBloodCellCharacter.generated.h"


class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class APlayerCamera; 
class  APlayerCameraManager;

UCLASS()
class BODYFRONTLINE_API AWhiteBloodCellCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AWhiteBloodCellCharacter();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void GetCursorPositionInThreeD();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void PostInitializeComponents() override;
	void SetOverlappingWeapon(AWeapon* Weapon);
	void PlayFireMontage();

	FORCEINLINE float GetAO_Pitch() const { return AO_Pitch; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* WhiteBloodCellMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* EquipAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* FireAction;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<APlayerCamera> CameraClass;

	UFUNCTION()
	void OnRep_OverlappingWeapon(AWeapon* LastWeapon);

	void Move(const FInputActionValue& value);
	void EPressed();
	void FireButton(const FInputActionValue& value);

	class AWeapon* OverlappingWeapon;

	float AO_Pitch;

	

private:
	//UPROPERTY(VisibleAnywhere)
	//USpringArmComponent* CameraBoom;

	//UPROPERTY(VisibleAnywhere)
	//UCameraComponent* ViewCamera;

	UPROPERTY(VisibleAnywhere)
	class UCombatComponent* Combat;

	const APlayerCameraManager* PlayerCameraManager;
	APlayerController* PlayerController;

	UPROPERTY(EditAnywhere, Category = "Combat")
	class UAnimMontage* FireWeaponMontage;
};
