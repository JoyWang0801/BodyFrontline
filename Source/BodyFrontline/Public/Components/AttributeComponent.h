// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/GameEnums.h"
#include "AttributeComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BODYFRONTLINE_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAttributeComponent();
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void ReceiveDamage(float Damage);
	float GetHealthPercent();
	int32 GetSoulsCount();
	int32 GetTimeCountdown();		// TODO - Will probably move to AI classes
	int32 GetWaveCount();			// TODO - Will probably move to AI classes
	bool IsAlive();
	void UpdateTimer();

	FTimerHandle GameTimer;

	FORCEINLINE void IncreaseSoul(int32 Number) { SoulsCount += Number; }
	FORCEINLINE void UpdateWave(int32 Wav) { WaveCount = Wav; }
	FORCEINLINE void SetDifficulty(EGameDifficulty diff) { GameDifficulty = diff; }

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:	
	
	UPROPERTY(EditAnywhere, Category = "Actor stats")
	float MaxHealth = 100.f;

	// Current health
	UPROPERTY(EditAnywhere, Category = "Actor stats")
	float Health = 100.f;

	UPROPERTY(EditAnywhere, Category = "Actor stats")
	int32 SoulsCount = 0;

	UPROPERTY(EditAnywhere, Category = "Actor stats")
	int32 TimeCountdown = 100;

	UPROPERTY(EditAnywhere, Category = "Actor stats")
	int32 WaveCount = 0;
		
	UPROPERTY(EditAnywhere, Category = "Actor stats")
	EGameDifficulty GameDifficulty = EGameDifficulty::EGD_Easy;
};