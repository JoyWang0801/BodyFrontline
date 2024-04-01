// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CellProjectile.generated.h"

UCLASS()
class BODYFRONTLINE_API ACellProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	ACellProjectile();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

private:	
	UPROPERTY(EditAnywhere)
	class USphereComponent* CollisionBall;

	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(EditAnywhere)
	class UParticleSystem* Tracer;

	class UParticleSystemComponent* TracerComponent;

	UPROPERTY(EditAnywhere)
	UParticleSystem* ImpactParticles;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	class USoundBase* OutSound;

	UPROPERTY(EditAnywhere, Category = "Weapon Properties")
	class USoundBase* ImpactSound;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	float Damage = 20.f;
};
