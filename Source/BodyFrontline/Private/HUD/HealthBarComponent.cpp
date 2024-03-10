// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/HealthBarComponent.h"
#include "HUD/HealthBar.h"
#include "Components/ProgressBar.h"

void UHealthBarComponent::SetHealthPercent(float Percent)
{
	if (HealthbarWidget == nullptr)
	{
		HealthbarWidget = Cast<UHealthBar>(GetUserWidgetObject());
	}
	if (HealthbarWidget && HealthbarWidget->HealthBar)
	{
		HealthbarWidget->HealthBar->SetPercent(Percent);
	}
}
