// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/WhiteBloodCellCharacter.h"
#include "Characters/PlayerCamera.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Characters/CombatComponent.h"
#include "Weapon/Weapon.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/PlayerCameraManager.h"
#include "Math/UnrealMathUtility.h"
#include "Characters/WBCAnimInstance.h"
#include "Components/AttributeComponent.h"
#include "Items/Item.h"
#include "HUD/HealthBarComponent.h"
#include "HUD/BodyFrontlineHUD.h"
#include "Materials/MaterialInterface.h"

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
	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));
	Attributes->SetCharacter(this);
	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBar"));
	HealthBarWidget->SetupAttachment(GetRootComponent());

	this->Tags.Add(FName("WBC"));
}

// Called when the game starts or when spawned
void AWhiteBloodCellCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController) 
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (Subsystem)
		{
			Subsystem->AddMappingContext(WhiteBloodCellMappingContext, 0);
		}

		PlayerController->bShowMouseCursor = true;
	}

	PlayerCameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);

	if (Attributes) 
	{
		GetWorldTimerManager().SetTimer(Attributes->GameTimer, this, &AWhiteBloodCellCharacter::UpdateTimerAttribute, 1.0f, true);
		Attributes->InitOverlay(PlayerController);
	}
}

void AWhiteBloodCellCharacter::Move(const FInputActionValue& value)
{
	const FVector2D MovementVector = value.Get<FVector2D>();
	if (GetController() && WBCState == ECharacterState::ECS_Alive)
	{
		const FRotator ControlRotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, ControlRotation.Yaw, 0.f);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X); // Left handed rule, x facing forward
		AddMovementInput(ForwardDirection, MovementVector.Y);

		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y); // left handed rule, y facing right
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AWhiteBloodCellCharacter::CharacterJump(const FInputActionValue& value)
{
	if (WBCState == ECharacterState::ECS_Alive)
	{
		ACharacter::Jump();
	}
}

void AWhiteBloodCellCharacter::EPressed()
{
	if (Combat && OverlappingWeapon != nullptr && WBCState == ECharacterState::ECS_Alive)
	{
		Combat->EquipWeapon(OverlappingWeapon);
	}
}

void AWhiteBloodCellCharacter::FireButton(const FInputActionValue& value)
{
	// 1 = click LMB, 0 = release LMB
	const bool actionValue = value.Get<bool>();
	if(WBCState == ECharacterState::ECS_Alive)
	{
		if (actionValue == 1) {if (Combat) {Combat->FireButtonPressed(true);}}
		else {if (Combat) {Combat->FireButtonPressed(false);}}
	}
}

void AWhiteBloodCellCharacter::UpdateTimerAttribute()
{
	if (Attributes && Attributes->PlayerOverlay)
	{
		//Attributes->UpdateTimer();
		//Attributes->PlayerOverlay->SetTimeCount(Attributes->GetTimeCountdown());

		// TODO - change to ftimehandler
		if (DmgIsBoosted) 
		{
			Attributes->UpdateItemEffectTimer();
			if (Attributes->GetItemEffectTimer() < 0)
			{
				DmgIsBoosted = false;
			}
		}
		if (WBCState == ECharacterState::ECS_Dead) 
		{
			Attributes->UpdateDeadTimer();
			//PlayerOverlay->SetCD(Attributes->GetDeadTimer());
			if (Attributes->GetDeadTimer() < 0) 
			{
				Reset();
			}
		}
		//if (Attributes->GetTimeCountdown() == 0)
		//{
		//	GameEnd();
		//}
	}
}

void AWhiteBloodCellCharacter::GameEnd(bool GameEndResult)
{
	if (Attributes->GameInstance)
	{
		Attributes->SetPlayerWin(GameEndResult);
	}
	UGameplayStatics::SetGamePaused(this, true);
	UGameplayStatics::OpenLevel(this, FName("GameEnd"));
}

void AWhiteBloodCellCharacter::Reset()
{
	if (Attributes && HealthBarWidget)
	{
		WBCState = ECharacterState::ECS_Alive;
		Attributes->ResetHealth();
		Attributes->ResetDeadTimer();
		GetMesh()->SetMaterial(0, WBCMaterial);
		HealthBarWidget->SetHealthPercent(Attributes->GetHealthPercent());
		HealthBarWidget->SetVisibility(true);
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

	GetCursorPositionInThreeD();
}

void AWhiteBloodCellCharacter::GetCursorPositionInThreeD()
{
	if (PlayerCameraManager && PlayerController)
	{
		FVector WorldLocation;
		FVector WorldDirection;
		const FVector CameraLocation = PlayerCameraManager->GetCameraLocation();

		if (PlayerController->DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
		{
			// get intersection point
			FVector LineEnd = WorldDirection * 1000.f + WorldLocation;
			FVector Intersection = FMath::LinePlaneIntersection(CameraLocation, LineEnd, GetActorLocation(), FVector(0.f, 1.f, 0.f));

			FHitResult OutHit;
			FVector Head = GetMesh()->GetSocketLocation("Head");

			// Trace a line from character's head to the cursor
			GetWorld()->LineTraceSingleByChannel(OutHit, Head, Intersection, ECollisionChannel::ECC_Visibility);
			FVector Distance = Intersection - Head;

			float GroundVelocity = GetCharacterMovement()->Velocity.X;
			AO_Pitch = Distance.Z / 2.0f; // Don't have any better way to do the calculation. TODO - Will need to adjust this
			if (Combat)
			{
				Combat->CrosshairPosition = Intersection;
				Combat->isInEyeSight = (GetActorForwardVector().X > 0 && Distance.X> 0) || (GetActorForwardVector().X < 0 && Distance.X < 0);
			}
		}
	}
}

// Called to bind functionality to input
void AWhiteBloodCellCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AWhiteBloodCellCharacter::Move);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AWhiteBloodCellCharacter::CharacterJump);
		EnhancedInputComponent->BindAction(EquipAction, ETriggerEvent::Triggered, this, &AWhiteBloodCellCharacter::EPressed);
		EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &AWhiteBloodCellCharacter::FireButton);
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
	OverlappingWeapon = Weapon;
}

void AWhiteBloodCellCharacter::PlayFireMontage()
{
	if (Combat == nullptr || Combat->EquippedWeapon == nullptr) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && FireWeaponMontage) 
	{
		AnimInstance->Montage_Play(FireWeaponMontage);
	}
}

void AWhiteBloodCellCharacter::UseItem(EItemType item)
{
	if (item == EItemType::EIT_HealPack) 
	{
		Attributes->AddHealth(35);
		HealthBarWidget->SetHealthPercent(Attributes->GetHealthPercent());
	}
	else if (item == EItemType::EIT_DamageBoost)
	{
		DmgIsBoosted = true;
		Attributes->ResetItemEffectTimer();
	}
}

void AWhiteBloodCellCharacter::RBCDie()
{
	if (Attributes) 
	{
		Attributes->UpdateRBCCount(-1);
	}

	if (Attributes->GetRBCCount() == 0)
	{
		GameEnd(false);
		/*UGameplayStatics::SetGamePaused(this, true);
		UGameplayStatics::OpenLevel(this, FName("GameEnd"));*/
	}
}

float AWhiteBloodCellCharacter::HealthPercent()
{
	return Attributes->GetHealthPercent();
}

int32 AWhiteBloodCellCharacter::WaveNumber()
{
	return Attributes->GetWaveCount();
}

int32 AWhiteBloodCellCharacter::RBCCount()
{
	return Attributes->GetRBCCount();
}

void AWhiteBloodCellCharacter::UpdateWaveNumber()
{
	Attributes->UpdateWave();
}

float AWhiteBloodCellCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (Attributes && HealthBarWidget && WBCState == ECharacterState::ECS_Alive)
	{
		Attributes->ReceiveDamage(DamageAmount);
		float CurrentHealthPercent = Attributes->GetHealthPercent();
		HealthBarWidget->SetHealthPercent(CurrentHealthPercent);

		if (!Attributes->IsAlive())
		{
			CharacterDie();
		}
	}

	return DamageAmount;
}

void AWhiteBloodCellCharacter::CharacterDie()
{
	WBCState = ECharacterState::ECS_Dead;
	HealthBarWidget->SetVisibility(false);
	PlayDeathMaterial();
}

void AWhiteBloodCellCharacter::SetOverlappingItem(AItem* Item)
{
	OverlappingItem = Item;
}

void AWhiteBloodCellCharacter::AddSouls(ASoul* Soul)
{
	Attributes->IncreaseSoul(1);
}

