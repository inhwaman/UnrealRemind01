// Fill out your copyright notice in the Description page of Project Settings.


#include "Drone.h"
#include "DroneController.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
ADrone::ADrone()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	SetRootComponent(BoxComp);
	BoxComp->SetSimulatePhysics(false);

	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComp"));
	StaticMeshComp->SetupAttachment(BoxComp);
	StaticMeshComp->SetSimulatePhysics(false);

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(BoxComp);
	SpringArmComp->TargetArmLength = 300.0f;
	SpringArmComp->bUsePawnControlRotation = false;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	Velocity = FVector::ZeroVector;
}
void ADrone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const FRotator CameraRotation = CameraComp->GetComponentRotation();
	const FRotator YawOnlyRotation(0.f, CameraRotation.Yaw, 0.f);

	const FVector CameraForward = FRotationMatrix(YawOnlyRotation).GetUnitAxis(EAxis::X);
	const FVector CameraRight = FRotationMatrix(YawOnlyRotation).GetUnitAxis(EAxis::Y);
	const FVector CameraUp = FRotationMatrix(YawOnlyRotation).GetUnitAxis(EAxis::Z);

	const FVector InputDirection = (CameraForward * MovementInput.X) + (CameraRight * MovementInput.Y)+ (CameraUp * MovementInput.Z);

	const FVector DesiredVelocity = InputDirection.GetSafeNormal() * MaxWalkSpeed;
	Velocity = FMath::VInterpTo(Velocity, DesiredVelocity, DeltaTime, VelocityInterpSpeed);

	AddActorWorldOffset(Velocity * DeltaTime,true);

	if(!FMath::IsNearlyZero(LookInput.X))
	{
		const float YawRotation = LookInput.X * TurnRate * DeltaTime;
		AddActorLocalRotation(FRotator(0.f, YawRotation, 0.f));
	}
	if (!FMath::IsNearlyZero(LookInput.Y))
	{
		FRotator CurrentArmRotation = SpringArmComp->GetRelativeRotation();
		float NewPitch = CurrentArmRotation.Pitch - (LookInput.Y * LookUpRate * DeltaTime);
		NewPitch = FMath::Clamp(NewPitch, MinPitch, MaxPitch);
		CurrentArmRotation.Pitch = NewPitch;
		SpringArmComp->SetRelativeRotation(CurrentArmRotation);
	}
}


// Called to bind functionality to input
void ADrone::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (ADroneController* DroneController = Cast<ADroneController>(GetController()))
		{
			if (DroneController->DroneMoveAction)
			{
				EnhancedInput->BindAction(
					DroneController->DroneMoveAction,
					ETriggerEvent::Triggered,
					this,
					&ADrone::MoveDrone
				);
			}
			if (DroneController->DroneMoveAction)
			{
				EnhancedInput->BindAction(
					DroneController->DroneMoveAction,
					ETriggerEvent::Completed,
					this,
					&ADrone::StopMoveDrone
				);
			}
			if (DroneController->DroneLookAction)
			{
				EnhancedInput->BindAction(
					DroneController->DroneLookAction,
					ETriggerEvent::Triggered,
					this,
					&ADrone::LookDrone
				);
			}
			if (DroneController->DroneLookAction)
			{
				EnhancedInput->BindAction(
					DroneController->DroneLookAction,
					ETriggerEvent::Completed,
					this,
					&ADrone::StopLookDrone
				);
			}
		}
	}


}

void ADrone::MoveDrone(const FInputActionValue& value)
{
	MovementInput = value.Get<FVector>();
}

void ADrone::StopMoveDrone(const FInputActionValue& value)
{
	MovementInput = FVector::ZeroVector;
}

void ADrone::LookDrone(const FInputActionValue& value)
{
	LookInput = value.Get<FVector2D>();
}

void ADrone::StopLookDrone(const FInputActionValue& value)
{
	LookInput = FVector2D::ZeroVector;
}

