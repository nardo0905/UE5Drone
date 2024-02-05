// Fill out your copyright notice in the Description page of Project Settings.


#include "DroneHUD.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

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

void UDroneHUD::SetAdvancedMode(bool bAdvanced)
{
	if (IsValid(AdvancedModeTextBlock))
	{
		if (bAdvanced)
		{
			AdvancedModeTextBlock->SetText(FText::FromString("ON"));
		}
		else
		{
			AdvancedModeTextBlock->SetText(FText::FromString("OFF"));
		}
	}
}

void UDroneHUD::SetFirstPersonMode(bool bIsInFirstPerson)
{
	if (IsValid(FirstPersonModeTextBlock))
	{
		if (bIsInFirstPerson)
		{
			FirstPersonModeTextBlock->SetText(FText::FromString("ON"));
		}
		else
		{
			FirstPersonModeTextBlock->SetText(FText::FromString("OFF"));
		}
	}
}
