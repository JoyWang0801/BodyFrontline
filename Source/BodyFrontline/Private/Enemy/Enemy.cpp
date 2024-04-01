// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/AttributeComponent.h"
#include "HUD/HealthBarComponent.h"
#include "Components/WidgetComponent.h"
#include "Weapon/Projectile.h"
#include "Items/Soul.h"
#include "Characters/WhiteBloodCellCharacter.h"

// Sets default values
AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore); // Likely not going to be triggered
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);

	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));
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


	if (this->ActorHasTag(FName("Enemy")))
	{
		// Tag exists on this object
		UE_LOG(LogTemp, Warning, TEXT("Tag 'Enemy' successfully added."));
	}
	else
	{
		// Tag does not exist on this object
		UE_LOG(LogTemp, Warning, TEXT("Tag 'Enemy' not found."));
	}
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (Attributes && HealthBarWidget)
	{
		Attributes->ReceiveDamage(DamageAmount);
		float CurrentHealthPercent = Attributes->GetHealthPercent();
		HealthBarWidget->SetHealthPercent(CurrentHealthPercent);

		if (!Attributes->IsAlive())
		{
			CurrentState = ECharacterState::ECS_Dead;
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			GetMesh()->SetVisibility(false);
			SetLifeSpan(1.0f);
			DeadWidget->SetVisibility(true);
			HealthBarWidget->SetVisibility(false);

			if (SoulClass)
			{
				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = GetOwner();
				UWorld* World = GetWorld();
				if (World)
				{
					ASoul* soul = World->SpawnActor<ASoul>(SoulClass, GetActorTransform());
				}
			}
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

