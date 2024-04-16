// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "HUD/HealthBarComponent.h"
#include "Components/WidgetComponent.h"
#include "Items/Soul.h"
#include "Items/Item.h"
#include "Items/HealPack.h"
#include "Items/DamageBoost.h"
#include "Characters/WhiteBloodCellCharacter.h"

// Sets default values
AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	//GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
	//GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(false);
	//GetMesh()->GetMaterial(0);

	//UCapsuleComponent* Capsule = GetCapsuleComponent();
	GetCapsuleComponent()->SetCollisionObjectType(ECollisionChannel::ECC_Pawn);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	//GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);
	//GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	HealthBarWidget = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBar"));
	HealthBarWidget->SetupAttachment(GetRootComponent());
	DeadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("DeadWidget"));
	DeadWidget->SetupAttachment(GetRootComponent());
	DeadWidget->SetVisibility(false);

	this->Tags.Add(FName("Enemy"));
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RunningTime += DeltaTime;

	if (CurrentState == ECharacterState::ECS_Alive)
	{
		AddActorWorldOffset(FVector(0.f, 0.f, TransformedSin()));
	}
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (HealthBarWidget)
	{
		ReceiveDamage(DamageAmount);
		float CurrentHealthPercent = GetHealthPercent();
		HealthBarWidget->SetHealthPercent(CurrentHealthPercent);

		if (!IsAlive())
		{
			Die();
		}
	}

	//if (DamageCauser && DamageCauser->GetOwner())
	//{
	//	AWhiteBloodCellCharacter* WBCCharacter = Cast<AWhiteBloodCellCharacter>(DamageCauser->GetOwner());
	//	if (WBCCharacter) 
	//	{
	//		// From weapon to hit location (cursor position)
	//		FVector ToTarget = WBCCharacter->GetActorLocation() - GetActorLocation();
	//		FRotator TargetRotation = ToTarget.Rotation();

	//		//FActorSpawnParameters SpawnParams;
	//		UWorld* World = GetWorld();
	//		if (World && ProjectileClass)
	//		{
	//			FActorSpawnParameters SpawnParams;
	//			SpawnParams.Owner = this;
	//			SpawnParams.Instigator = this;
	//			AProjectile* Projectile = World->SpawnActor<AProjectile>(ProjectileClass, GetActorLocation(), TargetRotation);
	//			UE_LOG(LogTemp, Warning, TEXT("Location %s."), *GetOwner()->GetName());
	//		}
	//	}
	//}

	return DamageAmount;
}

void AEnemy::Die()
{
	CurrentState = ECharacterState::ECS_Dead;
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetVisibility(false);
	SetLifeSpan(0.5f);
	DeadWidget->SetVisibility(true);
	HealthBarWidget->SetVisibility(false);

	DropRandom();
}

void AEnemy::DropRandom()
{
	UWorld* World = GetWorld();

	if (ItemClasses.Num() > 0)
	{
		int32 RandomNumber = FMath::RandRange(0, ItemClasses.Num());
		if (World)
		{
			if (RandomNumber == 0) 
			{
				World->SpawnActor<AHealPack>(ItemClasses[RandomNumber], GetActorTransform());
			}
			else if (RandomNumber == 1) 
			{
				World->SpawnActor<ADamageBoost>(ItemClasses[RandomNumber], GetActorTransform());
			}
			else if (RandomNumber > ItemClasses.Num() - 1) 
			{
				// not dropping any item
			}
		}
	}
	// Drop soul
	if (SoulClass)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = GetOwner();
		if (World)
		{
			ASoul* soul = World->SpawnActor<ASoul>(SoulClass, GetActorTransform());
		}
	}
}

void AEnemy::IncreaseExp(int32 exp)
{
	CurrentExp += exp;

	if (CurrentExp > NextLevelCost)
	{
		CurrentExp = CurrentExp - NextLevelCost;
		CurrentLevel++;
		NextLevelCost++;
	}

	UE_LOG(LogTemp, Warning, TEXT("CurrentLevel: %d."), CurrentLevel);
	if (CurrentLevel == 1 && EnemyMaterial.Num() > 1)
	{
		LevelUp(1);

	}
	else if (CurrentLevel == 2 && EnemyMaterial.Num() > 2)
	{
		LevelUp(2);

	}
	else if (CurrentLevel == 3 && EnemyMaterial.Num() > 3)
	{
		LevelUp(3);

	}
	else if (CurrentLevel == 4 && EnemyFinalFormMaterial.Num() > 0)
	{
		MaxHealth += 10.f;
		Health = MaxHealth;
		for (int i = 0; i < EnemyFinalFormMaterial.Num(); i++)
		{
			GetMesh()->SetMaterial(i, EnemyFinalFormMaterial[i]);
		}
	}
}

void AEnemy::ReceiveDamage(float Damage)
{
	Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);
}

float AEnemy::GetHealthPercent()
{
	return Health / MaxHealth;
}

bool AEnemy::IsAlive()
{
	return Health > 0.f;
}

void AEnemy::LevelUp(int32 level)
{
	GetMesh()->SetMaterial(0, EnemyMaterial[level - 1]);
	GetMesh()->SetMaterial(2, EnemyMaterial[level - 1]);

	// Increase Max HP and recover
	MaxHealth += 10.f;
	Health = MaxHealth;


}

