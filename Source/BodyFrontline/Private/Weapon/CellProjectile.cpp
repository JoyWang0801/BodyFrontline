// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/CellProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ACellProjectile::ACellProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionBall = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionBall"));
	SetRootComponent(CollisionBall);
	CollisionBall->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	CollisionBall->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionBall->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CollisionBall->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	CollisionBall->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->bRotationFollowsVelocity = true;

}

// Called when the game starts or when spawned
void ACellProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	if (Tracer)
	{
		TracerComponent = UGameplayStatics::SpawnEmitterAttached(Tracer, CollisionBall, FName(), GetActorLocation(), GetActorRotation(), EAttachLocation::KeepWorldPosition);
	}

	CollisionBall->OnComponentHit.AddDynamic(this, &ACellProjectile::OnHit);

	if (OutSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, OutSound, GetActorLocation());
	}
}

void ACellProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (ImpactParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, GetActorTransform());
	}
	if (ImpactSound)
	{
		//UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, Hit.Location);
	}

	if (Hit.GetActor())
	{
		// No ally damage
		bool HittingRBC = Hit.GetActor()->ActorHasTag(FName("RBC"));
		bool FromWBC = GetOwner()->ActorHasTag(FName("WBC"));
		if (!(HittingRBC && FromWBC))
		{
			UGameplayStatics::ApplyDamage(
				Hit.GetActor(),
				Damage,
				GetInstigator()->GetController(),
				this,
				UDamageType::StaticClass()
			);
		}
	}

	Destroy();
}

// Called every frame
void ACellProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

