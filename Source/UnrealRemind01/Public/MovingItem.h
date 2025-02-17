
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovingItem.generated.h"

UCLASS()
class UNREALREMIND01_API AMovingItem : public AActor
{
	GENERATED_BODY()
	
public:	
	AMovingItem();

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Item|Component")
	USceneComponent* SceneRoot;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Component")
	UStaticMeshComponent* StaticMeshComp;
	//������ ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Movement")
	float MoveSpeedX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Movement")
	float MoveSpeedY;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Movement")
	float MoveSpeedZ;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Movement")
	float MoveRangeX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Movement")
	float MoveRangeY;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Movement")
	float MoveRangeZ;
	//ȸ�� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Rotation")
	float RotateSpeedX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Rotation")
	float RotateSpeedY;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Rotation")
	float RotateSpeedZ;
	//�̵� �ð�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Second")
	float MoveSec;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Second")
	float StopMoveSec;
	//ȸ���ð�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Second")
	float RotateSec;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Second")
	float StopRotateSec;

	float MovementUpdateRate = 0.016f;

	FVector CurrentLocation;
	FVector MaxLocation;
	FVector MinLocation;
	//Ÿ�̸� �ڵ�
	FTimerHandle MovingTimerHandle;
	FTimerHandle StartMovingTimerHandle;
	FTimerHandle StopMovingTimerHandle;
	FTimerHandle RotateTimerHandle;
	FTimerHandle StartRotateTimerHandle;
	FTimerHandle StopRotateTimerHandle;


	virtual void BeginPlay() override;

	void MoveActor();
	void StartMoveActor();
	void StopMoveActor();

	void RotateActor();
	void StartRotateActor();
	void StopRotateActor();

};
