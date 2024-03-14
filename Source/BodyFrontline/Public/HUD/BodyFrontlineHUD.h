// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BodyFrontlineHUD.generated.h"

class UPlayerOverlay;
UCLASS()
class BODYFRONTLINE_API ABodyFrontlineHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	FORCEINLINE UPlayerOverlay* GetSlashOverlay() const { return PlayerOverlay; }

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = BodyFrontline)
	TSubclassOf<UPlayerOverlay> BodyFrontlineOverlayClass;

	UPROPERTY()
	UPlayerOverlay* PlayerOverlay;
};
