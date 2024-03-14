// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/PlayerOverlay.h"
#include "Components/TextBlock.h"

void UPlayerOverlay::SetSouls(int32 Soul)
{
	if (SoulCount) 
	{
		const FString SoulString = FString::Printf(TEXT("%d"), Soul);
		const FText SoulText = FText::FromString(SoulString);
		SoulCount->SetText(SoulText);
	}
}

void UPlayerOverlay::SetTimeCount(int32 Time)
{
	if (TimeCountdown)
	{
		TimeCountdown->SetText(FText::FromString(FString::Printf(TEXT("%d"), Time)));
	}
}

void UPlayerOverlay::SetWave(int32 Wav)
{
	if (WaveCount)
	{
		const FString WavString = FString("WAVE ") + FString::Printf(TEXT("%d"), Wav);
		const FText WavText = FText::FromString(WavString);
		WaveCount->SetText(WavText);
	}
}
