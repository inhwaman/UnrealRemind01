// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "EnhancedInputSubsystems.h"

AMainPlayerController::AMainPlayerController()
	:InputMappingContext(nullptr),
	MoveAction(nullptr),
	LookAction(nullptr),
	JumpAction(nullptr),
	SprintAction(nullptr)
{
}

void AMainPlayerController::BeginPlay()
{
	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())// ÇöÀç PlayerController¿¡ ¿¬°áµÈ Local Player °´Ã¼¸¦ °¡Á®¿È  
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())//LocalPlayer ¿¡¼­ EnhancedInputLocalPlayerSubsystemÀ» È¹µæ
		{
			if (InputMappingContext)
			{
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}

}
