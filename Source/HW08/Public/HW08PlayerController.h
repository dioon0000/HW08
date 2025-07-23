#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "HW08PlayerController.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class HW08_API AHW08PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AHW08PlayerController();

	// Variable
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputMappingContext* InputMappingContext;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputAction* SprintAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUD")
	TSubclassOf<UUserWidget> HUDWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD")
	UUserWidget* HUDWidgetInstance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu")
	TSubclassOf<UUserWidget> MainMenuWidgetClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Menu")
	UUserWidget* MainMenuWidgetInstance;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD")
	bool bIsEnd;

	// Function
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintPure, Category="HUD")
	UUserWidget* GetHUDWidget() const;
	UFUNCTION(BlueprintCallable, Category="HUD")
	void ShowGameHUD();
	UFUNCTION(BlueprintCallable, Category="Menu")
	void ShowMainMenu(bool bIsRestart);
	UFUNCTION(BlueprintCallable, Category="Menu")
	void StartGame();
};