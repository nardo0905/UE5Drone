// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DroneHUD.generated.h"

/**
 * 
 */
UCLASS()
class UE5TOPDOWNARPG_API UDroneHUD : public UUserWidget
{
	GENERATED_BODY()

private:

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* HealthBar;

	UPROPERTY(meta = (BindWidget))
	class URichTextBlock* SpeedTextBlock;

public:

	void SetHealth(float CurrentHealth, float MaxHealth);

	void SetSpeed(float NewSpeed);
	
};
