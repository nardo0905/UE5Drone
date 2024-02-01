// Fill out your copyright notice in the Description page of Project Settings.


#include "DroneHUD.h"

#include "Components/ProgressBar.h"
#include "Components/RichTextBlock.h"

void UDroneHUD::SetHealth(float CurrentHealth, float MaxHealth)
{
	if (IsValid(HealthBar))
	{
		HealthBar->SetPercent(CurrentHealth / MaxHealth);
	}
}

void UDroneHUD::SetSpeed(float NewSpeed)
{
	if (IsValid(SpeedTextBlock))
	{
		SpeedTextBlock->SetText(FText::AsNumber(NewSpeed));
	}
}
