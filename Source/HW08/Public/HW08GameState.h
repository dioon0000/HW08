#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "HW08GameState.generated.h"

UCLASS()
class HW08_API AHW08GameState : public AGameState
{
	GENERATED_BODY()
	
public:
	AHW08GameState();

	// Variable
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Score")
	int32 Score;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Coin")
	int32 SpawnedCoinCount;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Coin")
	int32 CollectedCoinCount;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Level")
	float LevelDuration;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Level")
	int32 CurrentLevelIndex;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Level")
	int32 MaxLevels;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Level")
	TArray<FName> LevelMapNames;
	// 현재 몇 단계의 웨이브가 진행 중인지 확인하는 정수 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Wave")
	int32 CurrentWaveIndex;

	FTimerHandle LevelTimerHandle;
	FTimerHandle HUDUpdateTimerHandle;
	// 다음 웨이브를 시작하기 위한 타이머를 관리하는 핸들
	FTimerHandle WaveTimerHandle;

	// Function
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintPure, Category="Score")
	int32 GetScore() const;
	UFUNCTION(BlueprintCallable, Category="Score")
	void AddScore(int32 Amount);
	UFUNCTION(BlueprintCallable, Category="Level")
	void OnGameOver();
	// 웨이브를 시작하기 위한 함수
	UFUNCTION(BlueprintCallable, Category="Wave")
	void StartWave();
	// 각 웨이브 별 타이머를 설정하는 역할을 위한 함수
	UFUNCTION(BlueprintCallable, Category="Wave")
	void SetupWaveTimer();

	void OnCoinCollected();
	void StartLevel();
	void OnLevelTimeUp();
	void EndLevel();
	void UpdateHUD();
};
