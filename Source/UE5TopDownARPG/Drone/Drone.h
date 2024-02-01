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

	void OnCanShootAgain();

protected:
	
	bool bCanShoot;

	FTimerHandle CanShootTimerHandle;

	void Death();

public:
	// Sets default values for this pawn's properties
	ADrone();

	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void TakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigateBy, AActor* DamageCauser);

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* CollisionMesh;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* Body;

	UPROPERTY(EditAnywhere)
	class USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere)
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere)
	class UFloatingPawnMovement* Movement; // floating helps smooth moving in air

	void Shoot();
	
	// is advanced flying mode on or off
	UPROPERTY()
	bool bAdvanced;

	// is the drone in first person mode
	UPROPERTY();
	bool bIsInFirstPerson;

	// current health
	UPROPERTY()
	float Health;

	// accumulate how much tilt should be applied to the body every tick
	UPROPERTY()
	float TiltInput;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ADroneShot> ShotClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> DroneDeathClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UDroneHUD> DroneHUDClass;

	UPROPERTY()
	class UDroneHUD* DroneHUD;

	UPROPERTY(EditAnywhere)
	float TimeBetweenShots;

	UPROPERTY(EditAnywhere)
	float MaxHealth;

	UPROPERTY(EditAnywhere);
	float DefaultSpringArmLenght;

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
	
};
