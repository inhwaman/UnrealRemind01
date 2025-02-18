
#include "PlayerCharacter.h"
#include "MainPlayerController.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFrameWork/CharacterMovementComponent.h"//GetCharacterMovement �������� ���� �������


APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	bUseControllerRotationYaw = false;//ĳ���Ͱ� ��Ʈ�ѷ��� ���� ȸ������ ����

	GetCharacterMovement()->bOrientRotationToMovement = true;//ĳ���Ͱ� ���� ���⿡ ���� ȸ��
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);//ĳ������ ȸ�� �ӵ�

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 350.0f;
	SpringArmComp->bUsePawnControlRotation = true;

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp,USpringArmComponent::SocketName);//������ ���� ���� ��ġ�� ����
	CameraComp->bUsePawnControlRotation = false;

	NormalSpeed = 600.0f;
	SprintMultiplier = 1.7f;
	SprintSpeed = NormalSpeed * SprintMultiplier;

}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))//ĳ����
	{
		if (AMainPlayerController* PlayerController = Cast<AMainPlayerController>(GetController()))//��Ʈ�ѷ� ĳ����(���� �����ִ� ��Ʈ�ѷ��� AMainPlayerController�� ĳ����)
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
	if (!Controller)return;//��Ʈ�ѷ� ������ �� ����
	const FVector2D MoveInput = value.Get<FVector2D>();
	if (MoveInput.IsNearlyZero())return;

	const FRotator ControlRotation = Controller->GetControlRotation();//��Ʈ�ѷ��� ȸ���� ������
	const FRotator YawRotation(0, ControlRotation.Yaw, 0);//������ ������ ��Ʈ�ѷ��� ȸ�� �� Yawȸ�� ���⸸ ������

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);//ī�޶� �������� ���� ���� ���͸� ���**
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);//ī�޶� �������� ������ ���� ���͸� ���**

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
	if (value.Get<bool>())//���ȴ��� �ƴ����� �޾ƿ�
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