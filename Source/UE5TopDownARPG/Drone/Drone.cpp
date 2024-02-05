// Fill out your copyright notice in the Description page of Project Settings.


#include "Drone.h"

#include "DroneController.h"
#include "UE5TopDownARPG/Drone/DroneShot/DroneShot.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystemInterface.h"
#include "Camera/CameraComponent.h"
#include "DroneHUD/DroneHUD.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/SpringArmComponent.h"
#include "UE5TopDownARPG/UE5TopDownARPG.h"

// Sets default values
ADrone::ADrone()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CollisionMesh"));
	SetRootComponent(CollisionMesh);
	
	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(CollisionMesh);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(CollisionMesh);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("DroneCamera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));

	// bool props
	bAdvanced = true;
	bIsInFirstPerson = false;
	bCanShoot = true;
	// spring arm
	DefaultSpringArmLenght = 200.f;
	// shaking and tilting
	ShakeFrequency = 2.f;
	ShakeAmplitude = 80.f;
	TiltMax = 15.f;
	TiltMoveScale = 0.3f;
	TiltRotateScale = 0.2;
	TiltResetScale = 0.5f;
	// shots
	TimeBetweenShots = 0.2f;
	// health stuff
	MaxHealth = 100.f;
	Health = MaxHealth;
	//after death spawn actor
	AfterDeathSpawnedActor = nullptr;
	HowLongShouldDeathActorStay = 5.f;

}

void ADrone::BeginPlay()
{
	Super::BeginPlay();

	// didnt work when i had it in the constructor for some reason?
	OnTakeAnyDamage.AddDynamic(this, &ADrone::TakeAnyDamage);

	// if (IsValid(DroneHUDClass))
	// {
	// 	ADroneController* DroneController = GetController<ADroneController>();
	// 	if (IsValid(DroneController))
	// 	{
	// 		UE_LOG(LogUE5TopDownARPG, Log, TEXT("VALID PLAYER CONTROLLER"));
	// 		DroneHUD = CreateWidget<UDroneHUD>(DroneController, DroneHUDClass);
	// 		if (IsValid(DroneHUD))
	// 		{
	// 			DroneHUD->AddToPlayerScreen();
	// 			DroneHUD->SetHealth(Health, MaxHealth);
	// 			DroneHUD->SetFirstPersonMode(bIsInFirstPerson);
	// 			DroneHUD->SetAdvancedMode(bAdvanced);
	// 		}
	// 	}
	// }
}

void ADrone::Death()
{

	if (DroneHUD)
	{
		DroneHUD->RemoveFromParent();
		DroneHUD = nullptr;
	}
	
	FVector Location = GetActorLocation();
	FRotator Rotation = GetActorRotation();
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	// handle after death spawned actor
	AfterDeathSpawnedActor = GetWorld()->SpawnActor(DroneDeathClass, &Location, &Rotation, SpawnParameters);
	// AfterDeathSpawnedActor->InitialLifeSpan = 2.f;
	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	// FTimerDelegate AfterDeathFunction = FTimerDelegate::CreateUObject(this, &ADrone::DestroyAfterDeathSpawnObject);
	TimerManager.SetTimer(DestroyAfterDeathSpawnObjectHandle, this, &ADrone::DestroyAfterDeathSpawnObject, HowLongShouldDeathActorStay);
	// TimerManager.SetTimer(DestroyAfterDeathSpawnObjectHandle, AfterDeathFunction, HowLongShouldDeathActorStay, false);

	UE_LOG(LogUE5TopDownARPG, Log, TEXT("DRONE DEATH"));
	Destroy();
}

void ADrone::DestroyAfterDeathSpawnObject()
{
	UE_LOG(LogUE5TopDownARPG, Log, TEXT("PARTICLE SHOULD BE DESTROYED"));
	if (IsValid(AfterDeathSpawnedActor))
	{
		AfterDeathSpawnedActor->Destroy();
		AfterDeathSpawnedActor = nullptr;
	}
}

void ADrone::OnCanShootAgain()
{
	bCanShoot = true;
}

// Called every frame
void ADrone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsValid(DroneHUD))
	{
		float Speed = GetVelocity().Length();
		// UE_LOG(LogUE5TopDownARPG, Log, TEXT("Speed %f"), speed);
		DroneHUD->SetSpeed(Speed);
	}

	// shaking when body is stationary so it looks like its flying in one place
	if (ShakeAmplitude)
	{
		ShakeTime += DeltaTime;
		// sin function to simulate up and down shaking of the drone
		float ShakeCurrent = FMath::Sin(ShakeTime * ShakeFrequency) * ShakeAmplitude;
		Body->SetRelativeLocation(FVector(0.f, 0.f, ShakeOffset + ShakeCurrent));
	}

	// handle tilting of body when turning and moving
	FRotator BodyRotation = Body->GetRelativeRotation();
	if (TiltInput)
	{
		BodyRotation.Roll += TiltInput;
		if (BodyRotation.Roll > TiltMax)
		{
			BodyRotation.Roll = TiltMax;
		}
		if (BodyRotation.Roll < -TiltMax)
		{
			BodyRotation.Roll = -TiltMax;
		}
		// prepare it for next tick
		TiltInput = 0.f;
	}

	// tilt back to center if there is body rotation
	if (BodyRotation.Roll > 0.f)
	{
		BodyRotation.Roll -= TiltResetScale;
		if (BodyRotation.Roll < 0.f)
		{
			BodyRotation.Roll = 0.f;
		}
	} else if (BodyRotation.Roll < 0.f)
	{
		BodyRotation.Roll += TiltResetScale;
		if (BodyRotation.Roll > 0.f)
		{
			BodyRotation.Roll = 0.f;
		}
	}

	Body->SetRelativeRotation(BodyRotation);

}

// Called to bind functionality to input
void ADrone::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	ADroneController* DC = Cast<ADroneController>(Controller);

	if (IsValid(EIC) && IsValid(DC))
	{
		UE_LOG(LogUE5TopDownARPG, Log, TEXT("VALID PLAYER CONTROLLER"));
		EIC->BindAction(DC->ShootAction, ETriggerEvent::Started, this, &ADrone::Shoot);
		// initializing hud here because controller is not yet ready in begin play i think
		// didnt print out log when i tried it there
		if (IsValid(DroneHUDClass))
		{
			DroneHUD = CreateWidget<UDroneHUD>(DC, DroneHUDClass);
			if (IsValid(DroneHUD))
			{
				DroneHUD->AddToPlayerScreen();
				DroneHUD->SetHealth(Health, MaxHealth);
				DroneHUD->SetFirstPersonMode(bIsInFirstPerson);
				DroneHUD->SetAdvancedMode(bAdvanced);
			}
		}
	}

}

void ADrone::TakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigateBy,
	AActor* DamageCauser)
{
	Health -= Damage;
	UE_LOG(LogUE5TopDownARPG, Log, TEXT("Health: %f"), Health);

	if (IsValid(DroneHUD))
	{
		DroneHUD->SetHealth(Health, MaxHealth);
	}

	if (Health <= 0.f)
	{
		Death();
	}
}

void ADrone::Shoot()
{
	if (!bCanShoot)
	{
		return;
	}

	bCanShoot = false;

	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	TimerManager.SetTimer(CanShootTimerHandle, this, &ADrone::OnCanShootAgain, TimeBetweenShots);
	
	UE_LOG(LogUE5TopDownARPG, Log, TEXT("SHOOT"));
	FTransform BodyTransform = Body->GetComponentTransform();
	FRotator Rotator = BodyTransform.Rotator();
	FVector ShotOffset(200.f, 0.f, 0.f);
	FVector Translation = BodyTransform.GetTranslation() + Rotator.RotateVector(ShotOffset);
	GetWorld()->SpawnActor<ADroneShot>(ShotClass, Translation, Rotator);
}
