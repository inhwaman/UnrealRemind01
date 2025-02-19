// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameState.h"
#include "Kismet/GameplayStatics.h"
#include "SpawnVolume.h"
#include "MainGameInstance.h"
#include "CoinItem.h"

AMainGameState::AMainGameState()
{
	Score = 0;
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;
	LevelDuration = 10.0f;
	CurrentLevelIndex = 0;
	MaxLevel = 2;
}

void AMainGameState::BeginPlay()
{
	Super::BeginPlay();
	StartLevel();
}

int32 AMainGameState::GetScore() const
{
	return Score;
}

void AMainGameState::AddScore(int32 Amount)
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UMainGameInstance* MainGameInstance = Cast<UMainGameInstance>(GameInstance);
		if (MainGameInstance)
		{
			MainGameInstance->AddToScore(Amount);
		}
	}
}


void AMainGameState::StartLevel()
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UMainGameInstance* MainGameInstance = Cast<UMainGameInstance>(GameInstance);
		if (MainGameInstance)
		{
			CurrentLevelIndex = MainGameInstance->CurrentLevelIndex;
		}
	}
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;

	TArray<AActor*> FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes);

	const int32 ItemToSpawn = 40;
	for (int32 i = 0; i < ItemToSpawn; i++)
	{
		if (FoundVolumes.Num() > 0)
		{
			ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundVolumes[0]);
			if (SpawnVolume)
			{
				AActor* SpawnedActor = SpawnVolume->SpawnRandomItem();
				if (SpawnedActor && SpawnedActor->IsA(ACoinItem::StaticClass()))
				{
					SpawnedCoinCount++;
				}
			}
		}
	}

	GetWorldTimerManager().SetTimer(
		LevelTimerHandle,
		this,
		&AMainGameState::OnLevelTimeUp,
		LevelDuration,
		false
	);
}

void AMainGameState::OnLevelTimeUp()
{
	UE_LOG(LogTemp, Warning, TEXT("Level Time Up!"));
	EndLevel();
}

void AMainGameState::OnCoinCollected()
{
	CollectedCoinCount++;
	if (SpawnedCoinCount > 0 && CollectedCoinCount >= SpawnedCoinCount)
	{
		EndLevel();
	}
}

void AMainGameState::EndLevel()
{
	UE_LOG(LogTemp, Warning, TEXT("EndLevel Called!"));
	GetWorldTimerManager().ClearTimer(LevelTimerHandle);

	CurrentLevelIndex++;
	UE_LOG(LogTemp, Warning, TEXT("CurrentLevelIndex: %d, MaxLevel: %d"), CurrentLevelIndex, MaxLevel);

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UMainGameInstance* MainGameInstance = Cast<UMainGameInstance>(GameInstance);
		if (MainGameInstance)
		{
			UE_LOG(LogTemp, Warning, TEXT("AddScore Called!"));
			AddScore(Score);
			MainGameInstance->CurrentLevelIndex = CurrentLevelIndex;
		}
	}
	if (CurrentLevelIndex >= MaxLevel)
	{
		UE_LOG(LogTemp, Warning, TEXT("OnGameOver Called!"));
		OnGameOver();
		return;
	}
	if (LevelMapNames.IsValidIndex(CurrentLevelIndex))
	{
		UE_LOG(LogTemp, Warning, TEXT("NextLevel Called!"));
		UGameplayStatics::OpenLevel(GetWorld(), LevelMapNames[CurrentLevelIndex]);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("LevelMapNames is invalid for CurrentLevelIndex: %d"), CurrentLevelIndex);
		OnGameOver();
	}
}

void AMainGameState::OnGameOver()
{
}