// Fill out your copyright notice in the Description page of Project Settings.


#include "MainGameState.h"
#include "MainGameInstance.h"
#include "MainPlayerController.h"
#include "SpawnVolume.h"
#include "CoinItem.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"


AMainGameState::AMainGameState()
	:Score(0),
	SpawnedCoinCount(0),
	CollectedCoinCount(0),
	CurrentLevelIndex(0),
	MaxLevel(2),
	MaxWave(3),
	WaveDuration(10.0f),
	CurrentWaveIndex(0),
	CoinsToSpawnPerWave({ 20,30,40 })
{
}

void AMainGameState::BeginPlay()
{
	Super::BeginPlay();
	StartLevel();

	GetWorldTimerManager().SetTimer(
		HUDUpdateTimerHandle,
		this,
		&AMainGameState::UpdateHUD,
		0.1f,
		true
	);
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
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (AMainPlayerController* MainPlayerController = Cast<AMainPlayerController>(PlayerController))
		{
			MainPlayerController->ShowGameHUD();
		}
	}

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UMainGameInstance* MainGameInstance = Cast<UMainGameInstance>(GameInstance);
		if (MainGameInstance)
		{
			CurrentLevelIndex = MainGameInstance->CurrentLevelIndex;
		}
	}
	CurrentWaveIndex = 0;
	StartWave();
}

void AMainGameState::OnCoinCollected()
{
	CollectedCoinCount++;
	if (SpawnedCoinCount > 0 && CollectedCoinCount >= SpawnedCoinCount)
	{
		EndWave();
	}
}

void AMainGameState::EndLevel()
{
	GetWorldTimerManager().ClearTimer(WaveTimerHandle);

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		UMainGameInstance* MainGameInstance = Cast<UMainGameInstance>(GameInstance);
		if (MainGameInstance)
		{
			AddScore(Score);
			CurrentLevelIndex++;
			MainGameInstance->CurrentLevelIndex = CurrentLevelIndex;
		}
	}
	if (CurrentLevelIndex >= MaxLevel)
	{
		OnGameOver();
		return;
	}
	if (LevelMapNames.IsValidIndex(CurrentLevelIndex))
	{
		UGameplayStatics::OpenLevel(GetWorld(), LevelMapNames[CurrentLevelIndex]);
	}
	else
	{
		OnGameOver();
	}
}

void AMainGameState::UpdateHUD()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		AMainPlayerController* MainPlayerController = Cast<AMainPlayerController>(PlayerController);
		{
			if (UUserWidget* HUDWidget = MainPlayerController->GetHUDWidget())
			{
				if (UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Time"))))
				{
					float RemainingTime = GetWorldTimerManager().GetTimerRemaining(WaveTimerHandle);
					TimeText->SetText(FText::FromString(FString::Printf(TEXT("Time: %.1f"), RemainingTime)));
				}
				if (UTextBlock* ScoreText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Score"))))
				{
					if (UGameInstance* GameInstance = GetGameInstance())
					{
						UMainGameInstance* MainGameInstance = Cast<UMainGameInstance>(GameInstance);
						if (MainGameInstance)
						{
							ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), MainGameInstance->TotalScore)));
						}
					}
				}
				if (UTextBlock* LevelIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Level"))))
				{
					LevelIndexText->SetText(FText::FromString(FString::Printf(TEXT("Level %d"), CurrentLevelIndex + 1)));
				}
				if (UTextBlock* WaveIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Wave"))))
				{
					WaveIndexText->SetText(FText::FromString(FString::Printf(TEXT("Wave: %d / %d"), CurrentWaveIndex + 1,MaxWave)));
				}
			}
		}
	}
}

void AMainGameState::StartWave()
{
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;

	for (AActor* Item : CurrentWaveItems)
	{
		if (Item && Item->IsValidLowLevelFast())
		{
			Item->Destroy();
		}
	}
	CurrentWaveItems.Empty();

	int32 CoinsToSpawn = (CoinsToSpawnPerWave.IsValidIndex(CurrentWaveIndex)) ? CoinsToSpawnPerWave[CurrentWaveIndex] : 20;

	if (ASpawnVolume* SpawnVolume = GetSpawnVolume())
	{
		for (int32 i = 0;i < CoinsToSpawn;i++)
		{
			if (AActor* SpawnedActor = SpawnVolume->SpawnRandomItem())
			{
				if (SpawnedActor->IsA(ACoinItem::StaticClass()))
				{
					SpawnedCoinCount++;
					CurrentWaveItems.Add(SpawnedActor);
				}
			}
		}
	}

	GetWorldTimerManager().SetTimer(
		WaveTimerHandle,
		this,
		&AMainGameState::OnWaveTimeUp,
		WaveDuration,
		false
	);
}

void AMainGameState::EndWave()
{
	GetWorldTimerManager().ClearTimer(WaveTimerHandle);

	CurrentWaveIndex++;
	if (CurrentWaveIndex >= MaxWave)
	{
		EndLevel();
	}
	else
	{
		StartWave();
	}
}

void AMainGameState::OnWaveTimeUp()
{
	EndWave();
}

void AMainGameState::OnGameOver()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (AMainPlayerController* MainPlayerController = Cast<AMainPlayerController>(PlayerController))
		{
			MainPlayerController->SetPause(true);
			MainPlayerController->ShowMainMenu(true);
		}
	}
}

ASpawnVolume* AMainGameState::GetSpawnVolume() const
{
	TArray<AActor*> FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes);
	return(FoundVolumes.Num() > 0) ? Cast<ASpawnVolume>(FoundVolumes[0]) : nullptr;
}
AMainPlayerController* AMainGameState::GetMainPlayerController() const
{
	return Cast<AMainPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
}

UMainGameInstance* AMainGameState::GetMainGameInstance() const
{
	return Cast<UMainGameInstance>(GetGameInstance());
}
