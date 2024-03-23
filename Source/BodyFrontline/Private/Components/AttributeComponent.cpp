// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/AttributeComponent.h"
#include "GameFramework/Actor.h"
#include "Interfaces/GameEnums.h"
#include "Interfaces/BodyFrontlineGameInstance.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UAttributeComponent::UAttributeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
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
	
}


// Called every frame
void UAttributeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
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

