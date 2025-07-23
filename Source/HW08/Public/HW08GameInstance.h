#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "HW08GameInstance.generated.h"

UCLASS()
class HW08_API UHW08GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UHW08GameInstance();

	// Variable
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	int32 TotalScore;
	// 현재 레벨 인덱스 (GameState에서도 관리할 수 있지만, 맵 전환 후에도 살리고 싶다면 GameInstance에 복제할 수 있음)
	// GameState에서 CurrentLevelIndex는 매번 초기화
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	int32 CurrentLevelIndex;

	// Function
	UFUNCTION(BlueprintCallable, Category = "GameData")
	void AddToScore(int32 Amount);
};