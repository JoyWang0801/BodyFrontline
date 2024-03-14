// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerOverlay.generated.h"

/**
 * 
 */
UCLASS()
class BODYFRONTLINE_API UPlayerOverlay : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetSouls(int32 Soul);
	void SetTimeCount(int32 Time);
	void SetWave(int32 Wav);

private:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SoulCount;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TimeCountdown;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* WaveCount;
};
