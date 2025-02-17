
#pragma once

#include "CoreMinimal.h"
#include "MovingItem.h"
#include "GameFramework/Actor.h"
#include "ItemSpawner.generated.h"

UCLASS()
class UNREALREMIND01_API AItemSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	AItemSpawner();
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "Spawn")
	int32 SpawnItemIndex;

protected:
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditAnywhere,Category = "Spawn")
	TSubclassOf<AMovingItem> ItemClass;
	void SpawnItem();
};
