
#include "PlayerCharacter.h"
#include "MainPlayerController.h"
#include "MainGameState.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFrameWork/CharacterMovementComponent.h"//GetCharacterMovement �������� ���� �������
#include "Components/WidgetComponent.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"


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
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);//������ ���� ���� ��ġ�� ����
	CameraComp->bUsePawnControlRotation = false;

	OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	OverheadWidget->SetupAttachment(GetMesh());
	OverheadWidget->SetWidgetSpace(EWidgetSpace::Screen);

	NormalSpeed = 600.0f;
	SprintMultiplier = 1.7f;
	SprintSpeed = NormalSpeed * SprintMultiplier;

	MaxHealth = 100.0f;
	Health = MaxHealth;

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

float APlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	Health = FMath::Clamp(Health - DamageAmount, 0.0f, MaxHealth);
	UpdateOverheadHP();

	if (Health <= 0.0f)
	{
		OnDeath();
	}
	return ActualDamage;
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	UpdateOverheadHP();
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

void APlayerCharacter::OnDeath()
{
	AMainGameState* MainGameState = GetWorld() ? GetWorld()->GetGameState<AMainGameState>() : nullptr;
	if (MainGameState)
	{
		MainGameState->OnGameOver();
	}
}


float APlayerCharacter::GetHealth() const
{
	return Health;
}

void APlayerCharacter::AddHealth(float Amount)
{
	Health = FMath::Clamp(Health + Amount, 0.0f, MaxHealth);
	UpdateOverheadHP();
}

void APlayerCharacter::UpdateOverheadHP()
{
	if (!OverheadWidget) return;

	UUserWidget* OverheadWidgetInstance = OverheadWidget->GetUserWidgetObject();
	if (!OverheadWidgetInstance)return;

	if (UProgressBar* HPBar = Cast<UProgressBar>(OverheadWidgetInstance->GetWidgetFromName(TEXT("OverheadHPBar"))))
	{
		const float HPpercent = (MaxHealth > 0.f) ? Health / MaxHealth : 0.f;
		HPBar->SetPercent(HPpercent);

		if (HPpercent < 0.3f)
		{
			HPBar->SetFillColorAndOpacity(FLinearColor::Red);
		}
	}
}