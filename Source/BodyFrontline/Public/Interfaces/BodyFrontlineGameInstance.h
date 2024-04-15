// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameEnums.h"
#include "BodyFrontlineGameInstance.generated.h"

/**
 * 
 */

UCLASS()
class BODYFRONTLINE_API UBodyFrontlineGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite)
	EGameDifficulty Diffculty;

	UPROPERTY(BlueprintReadWrite)
	bool bPlayerWin;
};
