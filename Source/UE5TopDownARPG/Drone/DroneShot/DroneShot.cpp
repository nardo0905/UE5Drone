// Fill out your copyright notice in the Description page of Project Settings.


#include "DroneShot.h"

#include "Components/SphereComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "UE5TopDownARPG/UE5TopDownARPG.h"

// Sets default values
ADroneShot::ADroneShot()
{
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SetRootComponent(SphereComponent);
	SphereComponent->SetSphereRadius(15.f);
	SphereComponent->OnComponentHit.AddDynamic(this, &ADroneShot::OnHit);
	SphereComponent->SetCollisionProfileName(TEXT("BlockAllDynamic"));

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("MovementComponent"));
	MovementComponent->InitialSpeed = 4000.f;
	MovementComponent->MaxSpeed = 4000.f;
	MovementComponent->ProjectileGravityScale = 0.f;

	InitialLifeSpan = 2.f;
}

void ADroneShot::OnHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogUE5TopDownARPG, Log, TEXT("HIT %s"), *Other->GetName());

	if (IsValid(Other))
	{
		Other->TakeDamage(Damage, FDamageEvent(UDamageType::StaticClass()), nullptr, this);
	}

	if (ShotDestroyClass)
	{
		FVector Location = GetActorLocation();
		FRotator Rotation = GetActorRotation();
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		GetWorld()->SpawnActor(ShotDestroyClass, &Location, &Rotation, SpawnParameters);
	}
	
	Destroy();
}

