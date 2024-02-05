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

public:

	void SetHealth(float CurrentHealth, float MaxHealth);

	void SetSpeed(float NewSpeed);

	void SetAdvancedMode(bool bAdvanced);

	void SetFirstPersonMode(bool bIsInFirstPerson);

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UProgressBar* HealthBar;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* SpeedTextBlock;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* AdvancedModeTextBlock;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* FirstPersonModeTextBlock;
	
};
