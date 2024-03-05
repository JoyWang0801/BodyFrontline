// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/WhiteBloodCellCharacter.h"
#include "Characters/PlayerCamera.h"
#include "Components/SphereComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Characters/CombatComponent.h"
#include "Weapon/Weapon.h"

// Sets default values
AWhiteBloodCellCharacter::AWhiteBloodCellCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;

	Combat = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));



}

// Called when the game starts or when spawned
void AWhiteBloodCellCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController) 
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (Subsystem)
		{
			Subsystem->AddMappingContext(WhiteBloodCellMappingContext, 0);
		}

		PlayerController->bShowMouseCursor = true;
	}

	//if (CameraClass) 
	//{
	//
	//	FVector SpawnLocation = GetActorLocation();
	//	FRotator SpawnRotation = FRotator(0.0f, 0.0f, 0.0f);
	//	FActorSpawnParameters SpawnParameters;
	//	// APlayerCamera* ViewCamera = GetWorld()->SpawnActor<APlayerCamera>(CameraClass, SpawnLocation, SpawnRotation, SpawnParameters);
	//	APlayerCamera* ViewCamera = GetWorld()->SpawnActor<APlayerCamera>(CameraClass);
	//}
}

void AWhiteBloodCellCharacter::Move(const FInputActionValue& value)
{
	const FVector2D MovementVector = value.Get<FVector2D>();
	if (GetController())
	{
		const FRotator ControlRotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X); // Left handed rule, x facing forward
		AddMovementInput(ForwardDirection, MovementVector.Y);

		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y); // left handed rule, y facing right
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AWhiteBloodCellCharacter::EPressed()
{
	if (Combat) 
	{
		Combat->EquipWeapon(OverlappingWeapon);
	}
}

void AWhiteBloodCellCharacter::OnRep_OverlappingWeapon(AWeapon* LastWeapon)
{
	if (OverlappingWeapon)
	{
		OverlappingWeapon->ShowPickupWidget(true);
	}
	if (LastWeapon)
	{
		LastWeapon->ShowPickupWidget(false);
	}
}

// Called every frame
void AWhiteBloodCellCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AWhiteBloodCellCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AWhiteBloodCellCharacter::Move);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(EquipAction, ETriggerEvent::Triggered, this, &AWhiteBloodCellCharacter::EPressed);
	}
}

void AWhiteBloodCellCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	if (Combat) 
	{
		Combat->Character = this;
	}
}

void AWhiteBloodCellCharacter::SetOverlappingWeapon(AWeapon* Weapon)
{
	//if (OverlappingWeapon)
	//{
	//	OverlappingWeapon->ShowPickupWidget(false);
	//}
	OverlappingWeapon = Weapon;
	UE_LOG(LogTemp, Warning, TEXT("Is Overlapping"));

	//if (IsLocallyControlled())
	//{
	//	if (OverlappingWeapon)
	//	{
	//		OverlappingWeapon->ShowPickupWidget(true);
	//	}
	//}
}

