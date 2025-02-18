
#include "PlayerCharacter.h"
#include "MainPlayerController.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFrameWork/CharacterMovementComponent.h"//GetCharacterMovement 가져오기 위한 헤더파일


APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	bUseControllerRotationYaw = false;//캐릭터가 컨트롤러에 따라 회전하지 않음

	GetCharacterMovement()->bOrientRotationToMovement = true;//캐릭터가 진행 방향에 맞춰 회전
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);//캐릭터의 회전 속도

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 350.0f;
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp,USpringArmComponent::SocketName);//스프링 암의 소켓 위치에 부착
	CameraComp->bUsePawnControlRotation = false;

	NormalSpeed = 600.0f;
	SprintMultiplier = 1.7f;
	SprintSpeed = NormalSpeed * SprintMultiplier;

}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))//캐스팅
	{
		if (AMainPlayerController* PlayerController = Cast<AMainPlayerController>(GetController()))//컨트롤러 캐스팅(지금 갖고있는 컨트롤러를 AMainPlayerController로 캐스팅)
		{
			if (PlayerController->MoveAction)//Move
			{
				EnhancedInput->BindAction(
					PlayerController->MoveAction,
					ETriggerEvent::Triggered,
					this,
					&APlayerCharacter::Move
				);
			}
			if (PlayerController->LookAction)//Look
			{
				EnhancedInput->BindAction(
					PlayerController->LookAction,
					ETriggerEvent::Triggered,
					this,
					&APlayerCharacter::Look
				);
			}
			if (PlayerController->JumpAction)//StartJump
			{
				EnhancedInput->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Triggered,
					this,
					&APlayerCharacter::StartJump
				);
			}
			if (PlayerController->MoveAction)//StopJump
			{
				EnhancedInput->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Completed,
					this,
					&APlayerCharacter::StopJump
				);
			}
			if (PlayerController->SprintAction)//StartSprint
			{
				EnhancedInput->BindAction(
					PlayerController->SprintAction,
					ETriggerEvent::Triggered,
					this,
					&APlayerCharacter::StartSprint
				);
			}
			if (PlayerController->SprintAction)//StopSprint
			{
				EnhancedInput->BindAction(
					PlayerController->SprintAction,
					ETriggerEvent::Completed,
					this,
					&APlayerCharacter::StopSprint
				);
			}
		}
	}


}

void APlayerCharacter::Move(const FInputActionValue& value)
{
	if (!Controller)return;//컨트롤러 없으면 일 암함
	const FVector2D MoveInput = value.Get<FVector2D>();
	if (MoveInput.IsNearlyZero())return;

	const FRotator ControlRotation = Controller->GetControlRotation();//컨트롤러의 회전을 가져옴
	const FRotator YawRotation(0, ControlRotation.Yaw, 0);//위에서 가져온 컨트롤러의 회전 중 Yaw회전 방향만 가져옴

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);//카메라 기준으로 전진 방향 백터를 계산**
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);//카메라 기준으로 오른쪽 방향 백터를 계산**

	if (!FMath::IsNearlyZero(MoveInput.X))
	{
		AddMovementInput(ForwardDirection, MoveInput.X);
	}
	if (!FMath::IsNearlyZero(MoveInput.Y))
	{
		AddMovementInput(RightDirection, MoveInput.Y);
	}
}

void APlayerCharacter::Look(const FInputActionValue& value)
{
	FVector2D LookInput = value.Get<FVector2D>();
	AddControllerYawInput(LookInput.X);
	AddControllerPitchInput(LookInput.Y);
}

void APlayerCharacter::StartJump(const FInputActionValue& value)
{
	if (value.Get<bool>())//눌렸는지 아닌지로 받아옴
	{
		Jump();
	}
}

void APlayerCharacter::StopJump(const FInputActionValue& value)
{
	if (!value.Get<bool>())
	{
		StopJumping();
	}
}

void APlayerCharacter::StartSprint(const FInputActionValue& value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}
}

void APlayerCharacter::StopSprint(const FInputActionValue& value)
{
	if (GetCharacterMovement())
	{
		GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	}
}