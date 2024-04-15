// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/GameEnums.h"
#include "AttributeComponent.generated.h"

#define DEATH_CD 5
#define ITEM_EFFECT_TIME_LEN 5
#define MAX_RBC 5

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
	void UpdateDeadTimer();
	void UpdateItemEffectTimer();
	// void UpdateTimer();
	void UpdateRBCCount(int32 amount);
	void AddHealth(int32 heal);
	void IncreaseSoul(int32 Number);
	void InitOverlay(APlayerController* PlayerController);
	void UpdateWave();
	void SetPlayerWin(bool b);

	FTimerHandle GameTimer;
	class UBodyFrontlineGameInstance* GameInstance;

	UPROPERTY()
	class UPlayerOverlay* PlayerOverlay;

	FORCEINLINE void SetDifficulty(EGameDifficulty diff) { GameDifficulty = diff; }
	FORCEINLINE void ResetHealth() { Health = MaxHealth; }
	FORCEINLINE void ResetDeadTimer() { DeadTimer = DEATH_CD; }
	FORCEINLINE int32 GetDeadTimer() const { return DeadTimer; }
	FORCEINLINE int32 GetRBCCount () const { return RBCCount; }
	FORCEINLINE void ResetItemEffectTimer() { ItemEffectTimer = DEATH_CD; }
	FORCEINLINE int32 GetItemEffectTimer() const { return ItemEffectTimer; }
	FORCEINLINE void SetCharacter(class AWhiteBloodCellCharacter* c) { Character = c; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE EGameDifficulty GetDifficulty() const { return GameDifficulty; }

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
	int32 TotalSoulsCount = 0;

	UPROPERTY(EditAnywhere, Category = "Actor stats")
	int32 TimeCountdown = 100;

	UPROPERTY(EditAnywhere, Category = "Actor stats")
	int32 WaveCount = 0;
	
	UPROPERTY(EditAnywhere, Category = "Actor stats")
	int32 RBCCount = 2;
		
	UPROPERTY(EditAnywhere, Category = "Actor stats")
	EGameDifficulty GameDifficulty = EGameDifficulty::EGD_Easy;

	int32 DeadTimer = DEATH_CD;
	int32 ItemEffectTimer = ITEM_EFFECT_TIME_LEN;

	class ABase* Base;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ARBCCharacter> RBCClass;

	AWhiteBloodCellCharacter* Character;

	int32 RBCGenerateSoulCount = 0;
	int32 RBCGenerateAmount = 2;
};