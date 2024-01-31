// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Engine/Canvas.h"
#include "DroneCrosshair.generated.h"

/**
 * 
 */
UCLASS()
class UE5TOPDOWNARPG_API ADroneCrosshair : public AHUD
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly)
	UTexture2D* CrosshairTexture;

public:

	virtual void DrawHUD() override;
	
};
