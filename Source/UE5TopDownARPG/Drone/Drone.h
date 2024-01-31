// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Drone.generated.h"

UCLASS()
class UE5TOPDOWNARPG_API ADrone : public APawn
{
	GENERATED_BODY()

private:

	// so i can calculate shaking
	float ShakeTime;
	
	float TiltInput;

	void OnCanShootAgain();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	bool bCanShoot;

	FTimerHandle CanShootTimerHandle;

public:
	// Sets default values for this pawn's properties
	ADrone();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* CollisionMesh;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* Body;

	UPROPERTY(EditAnywhere)
	class USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere)
	class UCameraComponent* Camera;

	void Move(const struct FInputActionValue& ActionValue);

	void Rotate(const struct FInputActionValue& ActionValue);

	void Shoot(const struct FInputActionValue& ActionValue);

	void ToggleAdvancedFlyMode();

	void ToggleFirstPerson();
	
	UPROPERTY(EditAnywhere)
	class UFloatingPawnMovement* Movement; // floating helps smooth moving in air
	
	UPROPERTY(EditAnywhere)
	float MoveScale;

	UPROPERTY(EditAnywhere)
	float RotateScale;

	// TODO: add hud element that displays this
	UPROPERTY()
	bool advanced;

	UPROPERTY();
	bool isInFirstPerson;

	UPROPERTY(EditAnywhere);
	float defaultSpringArmLenght;

	// Shake amplitude (strength). 0 is off
	UPROPERTY(EditAnywhere)
	float ShakeAmplitude;

	// How frequent should the shake be
	UPROPERTY(EditAnywhere)
	float ShakeFrequency;

	// offset to apply to Z coordinate after shaking
	UPROPERTY(EditAnywhere)
	float ShakeOffset;

	UPROPERTY(EditAnywhere)
	float TiltMax;

	UPROPERTY(EditAnywhere)
	float TiltMoveScale;

	UPROPERTY(EditAnywhere)
	float TiltRotateScale;

	UPROPERTY(EditAnywhere)
	float TiltResetScale;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ADroneShot> ShotClass;

	UPROPERTY(EditAnywhere)
	float TimeBetweenShots;
	
};
