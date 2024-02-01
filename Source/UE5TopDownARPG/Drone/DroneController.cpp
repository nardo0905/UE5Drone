// Fill out your copyright notice in the Description page of Project Settings.


#include "DroneController.h"

#include "Drone.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "GameFramework/SpringArmComponent.h"
#include "UE5TopDownARPG/UE5TopDownARPG.h"

class UEnhancedInputLocalPlayerSubsystem;

ADroneController::ADroneController()
{
	MoveScale = 1.f;
	RotateScale = 50.f;
}

void ADroneController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DroneMappingContext, 0);
	}
}

void ADroneController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADroneController::Move);
		EnhancedInputComponent->BindAction(RotateAction, ETriggerEvent::Triggered, this, &ADroneController::Rotate);
		EnhancedInputComponent->BindAction(AdvancedFlyOptionAction, ETriggerEvent::Started, this, &ADroneController::ToggleAdvancedFlyMode);
		EnhancedInputComponent->BindAction(ToggleFirstPersonAction, ETriggerEvent::Started, this, &ADroneController::ToggleFirstPerson);
	}
}

void ADroneController::Move(const FInputActionValue& ActionValue)
{
	ADrone* pawn = Cast<ADrone>(GetPawn());
	if (IsValid(pawn))
	{
		FVector Input = ActionValue.Get<FInputActionValue::Axis3D>();
		pawn->AddMovementInput(GetPawn()->GetActorRotation().RotateVector(Input), MoveScale);
		pawn->TiltInput += Input.Y * pawn->TiltMoveScale * MoveScale;
	}
}

void ADroneController::Rotate(const FInputActionValue& ActionValue)
{
	ADrone* pawn = Cast<ADrone>(GetPawn());
	if (IsValid(pawn))
	{
		FRotator Input(ActionValue[0], ActionValue[1], ActionValue[2]);
		// scale by time that has passed since last tick
		// done so mouse sensitivity can feel the same at different frame rates
		Input *= GetWorld()->GetDeltaSeconds() * RotateScale;
		pawn->TiltInput += Input.Yaw * pawn->TiltRotateScale;

		if (Cast<ADrone>(GetPawn())->bAdvanced)
		{
			pawn->AddActorLocalRotation(Input);
		} else
		{
			Input += GetPawn()->GetActorRotation();
			// clamping the angle so we cant loop around while looking up or down
			Input.Pitch = FMath::ClampAngle(Input.Pitch, -89.9f, 89.9f);
			Input.Roll = 0;
			pawn->SetActorRotation(Input);
		}
	}
}

void ADroneController::ToggleAdvancedFlyMode()
{
	ADrone* pawn = Cast<ADrone>(GetPawn());
	if (IsValid(pawn))
	{
		UE_LOG(LogUE5TopDownARPG, Log, TEXT("Toggle advance flying mode"));
		pawn->bAdvanced = !pawn->bAdvanced;
	}
}

void ADroneController::ToggleFirstPerson()
{
	ADrone* pawn = Cast<ADrone>(GetPawn());
	if (IsValid(pawn))
	{
		if (!pawn->bIsInFirstPerson)
		{
			UE_LOG(LogUE5TopDownARPG, Log, TEXT("First person mode ON"));
			pawn->bIsInFirstPerson = true;
			pawn->SpringArm->TargetArmLength = 0.f;
			pawn->SpringArm->bEnableCameraLag = false;
			pawn->SpringArm->bEnableCameraRotationLag = false;
		}
		else
		{
			UE_LOG(LogUE5TopDownARPG, Log, TEXT("First person mode OFF"));
			pawn->bIsInFirstPerson = false;
			pawn->SpringArm->TargetArmLength = pawn->DefaultSpringArmLenght;
			pawn->SpringArm->bEnableCameraLag = true;
			pawn->SpringArm->bEnableCameraRotationLag = true;
			pawn->SpringArm->CameraLagSpeed = 30.f;
			pawn->SpringArm->CameraRotationLagSpeed = 30.f;
			pawn->SpringArm->CameraLagMaxDistance = 50.f;
		}
	}
}
