// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/BodyFrontlineHUD.h"
#include "HUD/PlayerOverlay.h"

void ABodyFrontlineHUD::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (World) 
	{
		APlayerController* Controller = World->GetFirstPlayerController();
		if (Controller && BodyFrontlineOverlayClass)
		{
			PlayerOverlay = CreateWidget<UPlayerOverlay>(Controller, BodyFrontlineOverlayClass);
			PlayerOverlay->AddToViewport();
		}
	}
}
