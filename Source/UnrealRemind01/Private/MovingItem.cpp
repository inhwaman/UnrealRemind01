
#include "MovingItem.h"

AMovingItem::AMovingItem()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject <USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	StaticMeshComp = CreateDefaultSubobject <UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComp->SetupAttachment(SceneRoot);

	MoveSpeedX = FMath::RandRange(40.0f, -40.0f);
	MoveSpeedY = FMath::RandRange(40.0f, -40.0f);
	MoveSpeedZ = FMath::RandRange(40.0f, -40.0f);

	MoveRangeX = FMath::RandRange(200.0f, -200.0f);
	MoveRangeY = FMath::RandRange(200.0f, -200.0f);
	MoveRangeZ = FMath::RandRange(200.0f, -200.0f);

	RotateSpeedX = FMath::RandRange(40.0f, -40.0f);
	RotateSpeedY = FMath::RandRange(40.0f, -40.0f);
	RotateSpeedZ = FMath::RandRange(40.0f, -40.0f);

	MoveSec = RotateSec = FMath::RandRange(1.0f, 4.0f);
	StopMoveSec = StopRotateSec = FMath::RandRange(1.0f, 4.0f);
}

void AMovingItem::BeginPlay()
{
	Super::BeginPlay();
	CurrentLocation = GetActorLocation();
	MaxLocation = GetActorLocation() + FVector(MoveRangeX, MoveRangeY, MoveRangeZ);
	MinLocation = CurrentLocation;

	StartMoveActor();
	StartRotateActor();
}


void AMovingItem::MoveActor()
{
	CurrentLocation += FVector(MoveSpeedX, MoveSpeedY, MoveSpeedZ) * MovementUpdateRate;
	SetActorLocation(CurrentLocation);

	if (CurrentLocation.X >= MaxLocation.X || CurrentLocation.X <= MinLocation.X)
	{
		MoveSpeedX = -MoveSpeedX;
	}
	if (CurrentLocation.Y >= MaxLocation.Y || CurrentLocation.Y <= MinLocation.Y)
	{
		MoveSpeedY = -MoveSpeedY;
	}
	if (CurrentLocation.Z >= MaxLocation.Z || CurrentLocation.Z <= MinLocation.Z)
	{
		MoveSpeedZ = -MoveSpeedZ;
	}
}

void AMovingItem::StartMoveActor()
{
	GetWorld()->GetTimerManager().SetTimer(
		MovingTimerHandle,
		this,
		&AMovingItem::MoveActor,
		MovementUpdateRate,
		true
	);
	GetWorld()->GetTimerManager().SetTimer(
		StopMovingTimerHandle,
		this,
		&AMovingItem::StopMoveActor,
		MoveSec,
		false
	);
}

void AMovingItem::StopMoveActor()
{
	GetWorld()->GetTimerManager().ClearTimer(MovingTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(
		StartMovingTimerHandle,
		this,
		&AMovingItem::StartMoveActor,
		StopMoveSec,
		false
	);
}

void AMovingItem::RotateActor()
{
	AddActorLocalRotation(FRotator(RotateSpeedX * MovementUpdateRate, RotateSpeedY * MovementUpdateRate, RotateSpeedZ * MovementUpdateRate));

}

void AMovingItem::StartRotateActor()
{
	GetWorld()->GetTimerManager().SetTimer(
		RotateTimerHandle,
		this,
		&AMovingItem::RotateActor,
		MovementUpdateRate,
		true
	);
	GetWorld()->GetTimerManager().SetTimer(
		StopRotateTimerHandle,
		this,
		&AMovingItem::StopRotateActor,
		MoveSec,
		false
	);
}

void AMovingItem::StopRotateActor()
{
	GetWorld()->GetTimerManager().ClearTimer(RotateTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(
		StartRotateTimerHandle,
		this,
		&AMovingItem::StartRotateActor,
		StopRotateSec,
		false
	);
}


