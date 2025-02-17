

#include "ItemSpawner.h"

AItemSpawner::AItemSpawner()
{
	PrimaryActorTick.bCanEverTick = true;
	SpawnItemIndex = 10;
}

void AItemSpawner::BeginPlay()
{
	Super::BeginPlay();

	SpawnItem();
}

void AItemSpawner::SpawnItem()
{
	for (int32 i = 0;i < SpawnItemIndex;i++)
	{
		float RandomX = FMath::RandRange(-1500.0f, 1500.0f);
		float RandomY = FMath::RandRange(-1500.0f, 1500.0f);

		FVector SpawnLocation(RandomX, RandomY, 100.f);
		FRotator SpawnRotation(0.f, 0.f, 0.f);
		GetWorld()->SpawnActor<AMovingItem>(ItemClass, SpawnLocation, SpawnRotation);
	}
}


