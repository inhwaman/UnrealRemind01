// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Drone.generated.h"

class UBoxComponent;
class StaticMeshComponent;
class USpringArmComponent;
class UCameraComponent;

struct FInputActionValue;

UCLASS()
class UNREALREMIND01_API ADrone : public APawn
{
	GENERATED_BODY()

public:
	ADrone();
	//컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UBoxComponent* BoxComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UStaticMeshComponent* StaticMeshComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
	UCameraComponent* CameraComp;
	//이동
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MaxWalkSpeed = 600.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float TurnRate = 70.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float LookUpRate = 70.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MinPitch = -80.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MaxPitch = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float VelocityInterpSpeed = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	FVector Velocity;

	FVector MovementInput;
	FVector2D LookInput;

	virtual void Tick(float DeltaTime);

	UFUNCTION()
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UFUNCTION()
	void MoveDrone(const FInputActionValue& value);
	UFUNCTION()
	void StopMoveDrone(const FInputActionValue& value);
	UFUNCTION()
	void LookDrone(const FInputActionValue& value);
	UFUNCTION()
	void StopLookDrone(const FInputActionValue& value);
};
