
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "MainGameState.generated.h"

UCLASS()
class UNREALREMIND01_API AMainGameState : public AGameState
{
	GENERATED_BODY()
public:
	AMainGameState();

	virtual void BeginPlay()override;

	UPROPERTY(visibleAnywhere,BlueprintReadWrite,Category = "Score")
	int32 Score;
	UPROPERTY(visibleAnywhere, BlueprintReadOnly,Category = "Coin")
	int32 SpawnedCoinCount;
	UPROPERTY(visibleAnywhere, BlueprintReadOnly, Category = "Coin")
	int32 CollectedCoinCount;
	int32 MaxLevel;
	UPROPERTY(visibleAnywhere, BlueprintReadOnly, Category = "Level")
	float LevelDuration;
	UPROPERTY(visibleAnywhere, BlueprintReadOnly, Category = "Level")
	int32 CurrentLevelIndex;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
	TArray<FName> LevelMapNames;

	FTimerHandle LevelTimerHandle;

	UFUNCTION(blueprintPure, Category = "Score")
	int32 GetScore() const;
	UFUNCTION(blueprintCallable, Category = "Score")
	void AddScore(int32 Amount);
	UFUNCTION(blueprintCallable, Category = "Score")
	void OnGameOver();


	void StartLevel();
	void OnLevelTimeUp();
	void OnCoinCollected();
	void EndLevel();
};
