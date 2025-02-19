
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "MainGameState.generated.h"

class ASpawnVolume;
class AMainPlayerController;
class UMainGameInstance;

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
	int32 CurrentLevelIndex;
	//Wave
	UPROPERTY(visibleAnywhere, BlueprintReadOnly, Category = "Wave")
	int32 CurrentWaveIndex;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	int32 MaxWave;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	float WaveDuration;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Wave")
	TArray<int32> CoinsToSpawnPerWave;
	UPROPERTY()
	TArray<AActor*> CurrentWaveItems;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
	TArray<FName> LevelMapNames;

	FTimerHandle WaveTimerHandle;
	FTimerHandle HUDUpdateTimerHandle;

	UFUNCTION(blueprintPure, Category = "Score")
	int32 GetScore() const;
	UFUNCTION(blueprintCallable, Category = "Score")
	void AddScore(int32 Amount);
	UFUNCTION(blueprintCallable, Category = "Score")
	void OnGameOver();


	void StartLevel();
	void OnCoinCollected();
	void EndLevel();
	void UpdateHUD();

	void StartWave();
	void EndWave();
	void OnWaveTimeUp();

	ASpawnVolume* GetSpawnVolume()const;
	AMainPlayerController* GetMainPlayerController()const;
	UMainGameInstance* GetMainGameInstance()const;
};
