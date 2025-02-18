// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "DroneController.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class UNREALREMIND01_API ADroneController : public APlayerController
{
	GENERATED_BODY()
public:
	ADroneController();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputMappingContext* InputMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* DroneMoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	UInputAction* DroneLookAction;

	virtual void BeginPlay() override;
};
