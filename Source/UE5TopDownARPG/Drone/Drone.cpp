// Fill out your copyright notice in the Description page of Project Settings.


#include "Drone.h"

#include "DroneController.h"
#include "UE5TopDownARPG/Drone/DroneShot/DroneShot.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystemInterface.h"
#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"
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

	MoveScale = 1.f;
	RotateScale = 50.f;
	advanced = false;
	isInFirstPerson = false;
	defaultSpringArmLenght = 200.f;
	ShakeFrequency = 2.f;
	ShakeAmplitude = 80.f;
	TiltMax = 15.f;
	TiltMoveScale = 0.3f;
	TiltRotateScale = 0.2;
	TiltResetScale = 0.5f;
	bCanShoot = true;
	TimeBetweenShots = 0.2f;

}

void ADrone::OnCanShootAgain()
{
	bCanShoot = true;
}

// Called when the game starts or when spawned
void ADrone::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADrone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
		EIC->BindAction(DC->MoveAction, ETriggerEvent::Triggered, this, &ADrone::Move);
		EIC->BindAction(DC->RotateAction, ETriggerEvent::Triggered, this, &ADrone::Rotate);
		EIC->BindAction(DC->AdvancedFlyOptionAction, ETriggerEvent::Started, this, &ADrone::ToggleAdvancedFlyMode);
		EIC->BindAction(DC->ToggleFirstPersonAction, ETriggerEvent::Started, this, &ADrone::ToggleFirstPerson);
		EIC->BindAction(DC->ShootAction, ETriggerEvent::Started, this, &ADrone::Shoot);
	}

	ULocalPlayer* LocalPlayer = DC->GetLocalPlayer();

	if (IsValid(LocalPlayer))
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

		if (IsValid(Subsystem))
		{
			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(DC->DroneMappingContext, 0);
		}
		
	}

}

void ADrone::Move(const FInputActionValue& ActionValue)
{
	FVector Input = ActionValue.Get<FInputActionValue::Axis3D>();
	AddMovementInput(GetActorRotation().RotateVector(Input), MoveScale);
	TiltInput += Input.Y * TiltMoveScale * MoveScale;
}

void ADrone::Rotate(const FInputActionValue& ActionValue)
{
	FRotator Input(ActionValue[0], ActionValue[1], ActionValue[2]);
	// scale by time that has passed since last tick
	// done so mouse sensitivity can feel the same at different frame rates
	Input *= GetWorld()->GetDeltaSeconds() * RotateScale;
	TiltInput += Input.Yaw * TiltRotateScale;

	if (advanced)
	{
		AddActorLocalRotation(Input);
	} else
	{
		Input += GetActorRotation();
		// clamping the angle so we cant loop around while looking up or down
		Input.Pitch = FMath::ClampAngle(Input.Pitch, -89.9f, 89.9f);
		Input.Roll = 0;
		SetActorRotation(Input);
	}
}

void ADrone::Shoot(const FInputActionValue& ActionValue)
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

void ADrone::ToggleAdvancedFlyMode()
{
	UE_LOG(LogUE5TopDownARPG, Log, TEXT("Toggle advance flying mode"));
	advanced = !advanced;
}

void ADrone::ToggleFirstPerson()
{
	if (!isInFirstPerson)
	{
		UE_LOG(LogUE5TopDownARPG, Log, TEXT("First person mode ON"));
		isInFirstPerson = true;
		SpringArm->TargetArmLength = 0.f;
		SpringArm->bEnableCameraLag = false;
		SpringArm->bEnableCameraRotationLag = false;
	}
	else
	{
		UE_LOG(LogUE5TopDownARPG, Log, TEXT("First person mode OFF"));
		isInFirstPerson = false;
		SpringArm->TargetArmLength = defaultSpringArmLenght;
		SpringArm->bEnableCameraLag = true;
		SpringArm->bEnableCameraRotationLag = true;
		SpringArm->CameraLagSpeed = 30.f;
		SpringArm->CameraRotationLagSpeed = 30.f;
		SpringArm->CameraLagMaxDistance = 50.f;
	}
}


