#include "HW08GameState.h"
#include "HW08GameInstance.h"
#include "HW08PlayerController.h"
#include "HW08Character.h"
#include "Kismet/GameplayStatics.h"
#include "SpawnVolume.h"
#include "CoinItem.h"
#include "Components/TextBlock.h"
#include "Blueprint/UserWidget.h"

AHW08GameState::AHW08GameState()
{
	Score = 0;
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;
	LevelDuration = 60.0f;
	CurrentLevelIndex = 0;
	MaxLevels = 3;
	// 현재 웨이브 정보를 0으로 초기화
	CurrentWaveIndex = 0;
}

void AHW08GameState::BeginPlay()
{
	Super::BeginPlay();

	// 게임 시작 시 첫 레벨부터 진행
	StartLevel();

	GetWorldTimerManager().SetTimer(
		HUDUpdateTimerHandle,
		this,
		&AHW08GameState::UpdateHUD,
		0.1f,
		true
	);
}

int32 AHW08GameState::GetScore() const
{
    return Score;
}

void AHW08GameState::AddScore(int32 Amount)
{
	if(UGameInstance* GameInstance = GetGameInstance())
	{
		UHW08GameInstance* HW08GameInstance = Cast<UHW08GameInstance>(GameInstance);
		if(HW08GameInstance)
		{
			HW08GameInstance->AddToScore(Amount);
		}
	}
}

void AHW08GameState::OnGameOver()
{
	if(APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if(AHW08PlayerController* HW08PlayerController = Cast<AHW08PlayerController>(PlayerController))
		{
			HW08PlayerController->SetPause(true);
			HW08PlayerController->ShowMainMenu(true);
		}
	}
}

// 웨이브를 시작시키는 함수.
// UE_LOG, AddOnScreenDebugMessage를 사용하여 "Wave {num} 시작!" 로그 확인하기
// CurrentWaveIndex가 정상적으로 올라가는 지 확인하기
void AHW08GameState::StartWave()
{

	UE_LOG(LogTemp, Warning, TEXT("Wave %d 시작!"), CurrentWaveIndex);
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, FString::Printf(TEXT("Wave %d 시작!"), CurrentWaveIndex));

	// 첫번째 웨이브 작성
	if(CurrentWaveIndex == 1)
	{
		GetWorldTimerManager().SetTimer(
			WaveTimerHandle,
			this,
			&AHW08GameState::SetupWaveTimer,
			5.0f,
			false
		);
	}
	// 두번째 웨이브 작성
	else if(CurrentWaveIndex == 2)
	{
		GetWorldTimerManager().SetTimer(
			WaveTimerHandle,
			this,
			&AHW08GameState::SetupWaveTimer,
			10.0f,
			false
		);
	}
	// 세번째 웨이브 작성
	else if(CurrentWaveIndex == 3)
	{

	}
}

// 웨이브를 변경해주는 함수
void AHW08GameState::SetupWaveTimer()
{
	CurrentWaveIndex++;

	// 최대 웨이브 수는 3 웨이브로 설정
	if(CurrentWaveIndex <= 3)
	{
		StartWave();
	}
}

void AHW08GameState::OnCoinCollected()
{
	CollectedCoinCount++;

	UE_LOG(LogTemp, Warning, TEXT("Coin Collected: %d / %d"),
		   CollectedCoinCount,
		   SpawnedCoinCount)

		// 현재 레벨에서 스폰된 코인을 전부 주웠다면 즉시 레벨 종료
		if(SpawnedCoinCount > 0 && CollectedCoinCount >= SpawnedCoinCount)
		{
			EndLevel();
		}
}

void AHW08GameState::StartLevel()
{
	if(APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if(AHW08PlayerController* HW08PlayerController = Cast<AHW08PlayerController>(PlayerController))
		{
			HW08PlayerController->ShowGameHUD();
		}
	}

	if(UGameInstance* GameInstance = GetGameInstance())
	{
		UHW08GameInstance* HW08GameInstance = Cast<UHW08GameInstance>(GameInstance);
		if(HW08GameInstance)
		{
			CurrentLevelIndex = HW08GameInstance->CurrentLevelIndex;
		}
	}

	// 레벨 시작 시, 코인 개수 초기화
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;
	// 레벨 시작 시, 웨이브 번호 초기화
	CurrentWaveIndex = 0;

	// 현재 맵에 배치된 모든 SpawnVolume을 찾아 아이템 40개를 스폰
	TArray<AActor*> FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes);

	const int32 ItemToSpawn = 40;

	for(int32 i = 0; i < ItemToSpawn; i++)
	{
		if(FoundVolumes.Num() > 0)
		{
			ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundVolumes[0]);
			if(SpawnVolume)
			{
				AActor* SpawnedActor = SpawnVolume->SpawnRandomItem();
				// 만약 스폰된 액터가 코인 타입이라면 SpawnedCoinCount 증가
				if(SpawnedActor && SpawnedActor->IsA(ACoinItem::StaticClass()))
				{
					SpawnedCoinCount++;
				}
			}
		}
	}

	// LevelDuration초 후에 OnLevelTimeUp()가 호출되도록 타이머 설정
	GetWorldTimerManager().SetTimer(
		LevelTimerHandle,
		this,
		&AHW08GameState::OnLevelTimeUp,
		LevelDuration,
		false
	);
	// 웨이브를 시작하기 위해 SetupWaveTimer 함수 호출
	SetupWaveTimer();

	UE_LOG(LogTemp, Warning, TEXT("Level %d Start!, Spawned %d coin"),
		   CurrentLevelIndex + 1,
		   SpawnedCoinCount);
}

void AHW08GameState::OnLevelTimeUp()
{
	// 시간이 다 되면 레벨을 종료
	EndLevel();
}

void AHW08GameState::EndLevel()
{
	GetWorldTimerManager().ClearTimer(LevelTimerHandle);
	// 웨이브의 타이머 초기화
	GetWorldTimerManager().ClearTimer(WaveTimerHandle);
	CurrentLevelIndex++;

	if(UGameInstance* GameInstance = GetGameInstance())
	{
		UHW08GameInstance* HW08GameInstance = Cast<UHW08GameInstance>(GameInstance);
		if(HW08GameInstance)
		{
			AddScore(Score);
			HW08GameInstance->CurrentLevelIndex = CurrentLevelIndex;
		}
	}

	if(CurrentLevelIndex >= MaxLevels)
	{
		OnGameOver();
		return;
	}

	if(LevelMapNames.IsValidIndex(CurrentLevelIndex))
	{
		UE_LOG(LogTemp, Warning, TEXT("CurrentLevelIndex : %d"), CurrentLevelIndex);
		UGameplayStatics::OpenLevel(GetWorld(), LevelMapNames[CurrentLevelIndex]);
	}
	else
	{
		OnGameOver();
	}
}

void AHW08GameState::UpdateHUD()
{
	if(APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if(AHW08PlayerController* HW08PlayerController = Cast<AHW08PlayerController>(PlayerController))
		{
			if(UUserWidget* HUDWidght = HW08PlayerController->GetHUDWidget())
			{
				if(UTextBlock* TimeText = Cast<UTextBlock>(HUDWidght->GetWidgetFromName(TEXT("Time"))))
				{
					float RemainingTime = GetWorldTimerManager().GetTimerRemaining(LevelTimerHandle);
					TimeText->SetText(FText::FromString(
						FString::Printf(TEXT("Time: %.1f"), RemainingTime)));
				}

				if(UTextBlock* ScoreText = Cast<UTextBlock>(HUDWidght->GetWidgetFromName(TEXT("Score"))))
				{
					if(UGameInstance* GameInstance = GetGameInstance())
					{
						UHW08GameInstance* HW08GameInstance = Cast<UHW08GameInstance>(GameInstance);
						if(HW08GameInstance)
						{
							ScoreText->SetText(FText::FromString(
								FString::Printf(TEXT("Score: %d"),HW08GameInstance->TotalScore)));
						}
					}
				}

				if(UTextBlock* LevelIndexText = Cast<UTextBlock>(HUDWidght->GetWidgetFromName(TEXT("Level"))))
				{
					LevelIndexText->SetText(FText::FromString(
						FString::Printf(TEXT("Level: %d"), CurrentLevelIndex+1)));
				}

				// HP 텍스트를 Widget에 추가하기 위한 조건문
				if(UTextBlock* HPText = Cast<UTextBlock>(HUDWidght->GetWidgetFromName(TEXT("HP"))))
				{
					ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

					if(PlayerCharacter)
					{
						AHW08Character* HW08Character = Cast<AHW08Character>(PlayerCharacter);
						if(HW08Character)
						{
							HPText->SetText(FText::FromString(
								FString::Printf(TEXT("HP : %.0f"), HW08Character->GetHealth())
							));
						}
					}
				}
			}
		}
	}
}