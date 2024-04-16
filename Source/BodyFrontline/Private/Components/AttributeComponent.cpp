// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/AttributeComponent.h"
#include "GameFramework/Actor.h"
#include "Interfaces/GameEnums.h"
#include "Interfaces/BodyFrontlineGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Items/Base.h"
#include "Characters/WhiteBloodCellCharacter.h"
#include "Characters/RBCCharacter.h"
#include "Components/WidgetComponent.h"
#include "HUD/BodyFrontlineHUD.h"
#include "HUD/PlayerOverlay.h"

// Sets default values for this component's properties
UAttributeComponent::UAttributeComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UAttributeComponent::BeginPlay()
{
	Super::BeginPlay();

	GameInstance = Cast<UBodyFrontlineGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (GameInstance)
	{
		GameDifficulty = GameInstance->Diffculty;
	}
	
	int32 rbctemp = 0;
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
				//break;
			}
			if (Actor && Actor->ActorHasTag(FName("RBC")))
			{
				rbctemp++;
			}
		}
		RBCCount = rbctemp;
	}
}

void UAttributeComponent::InitOverlay(APlayerController* PlayerController)
{
	ABodyFrontlineHUD* BF_HUD = Cast<ABodyFrontlineHUD>(PlayerController->GetHUD());
	if (BF_HUD)
	{
		PlayerOverlay = BF_HUD->GetSlashOverlay();
		if (PlayerOverlay)
		{
			PlayerOverlay->SetWave(GetWaveCount());
			// PlayerOverlay->SetTimeCount(GetTimeCountdown());
			PlayerOverlay->SetSouls(GetSoulsCount());
			PlayerOverlay->SetRBCCount(GetRBCCount());
		}
	}
}

void UAttributeComponent::UpdateWave()
{
	WaveCount++; 
	if (PlayerOverlay)
	{
		PlayerOverlay->SetWave(GetWaveCount());
	}
}

void UAttributeComponent::SetPlayerWin(bool b)
{
	if(GameInstance)
	{ GameInstance->bPlayerWin = b; }
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
	return TotalSoulsCount;
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

	 PlayerOverlay->SetCD(GetDeadTimer());
}

void UAttributeComponent::UpdateItemEffectTimer()
{
	ItemEffectTimer = FMath::Clamp(ItemEffectTimer - 1, -1.f, ITEM_EFFECT_TIME_LEN);
}

//void UAttributeComponent::UpdateTimer()
//{
//	TimeCountdown = FMath::Clamp(TimeCountdown - 1, 0.f, 100.f);
//	PlayerOverlay->SetTimeCount(GetTimeCountdown());
//}

void UAttributeComponent::UpdateRBCCount(int32 amount)
{
	RBCCount = FMath::Clamp(RBCCount + amount, 0, MAX_RBC);
	PlayerOverlay->SetRBCCount(GetRBCCount());
}

void UAttributeComponent::AddHealth(int32 heal)
{
	Health = FMath::Clamp(Health + heal, 0.f, MaxHealth);
}

void UAttributeComponent::IncreaseSoul(int32 Number)
{  
	TotalSoulsCount += Number; 
	RBCGenerateSoulCount += Number;
	if (RBCGenerateSoulCount % RBCGenerateAmount == 0)
	{
		UWorld* World = GetWorld();
		if (World && Base && RBCCount < MAX_RBC)
		{
			World->SpawnActor<ARBCCharacter>(RBCClass, Base->GetActorTransform());
			RBCCount++;
			PlayerOverlay->SetRBCCount(GetRBCCount());
			RBCGenerateSoulCount = 0;
			//RBCGenerateAmount++;
		}
	}

	PlayerOverlay->SetSouls(GetSoulsCount());
}

