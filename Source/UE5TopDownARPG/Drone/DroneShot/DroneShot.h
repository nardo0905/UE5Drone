// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DroneShot.generated.h"

UCLASS()
class UE5TOPDOWNARPG_API ADroneShot : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADroneShot();

	UFUNCTION()
	void OnHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp,
		FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(EditAnywhere)
	class USphereComponent* SphereComponent;

	UPROPERTY(EditAnywhere)
	class UProjectileMovementComponent* MovementComponent;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ShotDestroyClass;

	UPROPERTY(EditAnywhere)
	float Damage = 20.f;
};
