// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/AttributeComponent.h"
#include "GameFramework/Actor.h"
#include "Interfaces/GameEnums.h"
#include "Interfaces/BodyFrontlineGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Items/Base.h"
#include "Characters/RBCCharacter.h"
#include "Components/WidgetComponent.h"

// Sets default values for this component's properties
UAttributeComponent::UAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	UBodyFrontlineGameInstance* GameInstance = Cast<UBodyFrontlineGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance)
	{
		GameDifficulty = GameInstance->Diffculty;
	}
	
	UWorld* world = GetWorld();
	if (world)
	{
		TArray<AActor*> TempActors;
		UGameplayStatics::GetAllActorsOfClass(world, AActor::StaticClass(), TempActors);
		for (AActor* Actor : TempActors)
		{
			if (Actor && Actor->ActorHasTag(FName("Base")))
			{
				Base = Cast<ABase>(Actor);
			}
		}
	}
}

void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UAttributeComponent::ReceiveDamage(float Damage)
{
	Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);
}

float UAttributeComponent::GetHealthPercent()
{
	return Health/MaxHealth;
}

int32 UAttributeComponent::GetSoulsCount()
{
	return SoulsCount;
}


int32 UAttributeComponent::GetTimeCountdown()
{
	return TimeCountdown;
}

int32 UAttributeComponent::GetWaveCount()
{
	return WaveCount;
}

bool UAttributeComponent::IsAlive()
{
	return Health > 0.f;
}

void UAttributeComponent::UpdateDeadTimer()
{
	 DeadTimer = FMath::Clamp(DeadTimer - 1, -1.f, DEATH_CD);
}

void UAttributeComponent::UpdateItemEffectTimer()
{
	ItemEffectTimer = FMath::Clamp(ItemEffectTimer - 1, -1.f, ITEM_EFFECT_TIME_LEN);
}

void UAttributeComponent::UpdateTimer()
{
	TimeCountdown = FMath::Clamp(TimeCountdown - 1, 0.f, 100.f);
}

void UAttributeComponent::AddHealth(int32 heal)
{
	Health = FMath::Clamp(Health + heal, 0.f, MaxHealth);
}

void UAttributeComponent::IncreaseSoul(int32 Number)
{  
	SoulsCount += Number; 
	if (SoulsCount == 5) 
	{
		UE_LOG(LogTemp, Warning, TEXT("Time to spawn a oxygen"));
	}
}

