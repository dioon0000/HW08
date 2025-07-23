#include "HW08PlayerController.h"
#include "HW08GameState.h"
#include "HW08GameInstance.h"
#include "EnhancedInputSubsystems.h" // Enhanced Input System의 Local Player Subsystem을 사용하기 위해 포함
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

AHW08PlayerController::AHW08PlayerController() :
    InputMappingContext(nullptr),
    MoveAction(nullptr),
    JumpAction(nullptr),
    LookAction(nullptr),
    SprintAction(nullptr),
    HUDWidgetClass(nullptr),
    HUDWidgetInstance(nullptr),
    MainMenuWidgetClass(nullptr),
    MainMenuWidgetInstance(nullptr)
{
    bIsEnd = false;
}

void AHW08PlayerController::BeginPlay()
{
	Super::BeginPlay();

	if(ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if(UEnhancedInputLocalPlayerSubsystem* Subsystem =
		   LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}

	FString CurrentMapName = GetWorld()->GetMapName();
	if(CurrentMapName.Contains("MenuLevel"))
	{
		ShowMainMenu(false);
	}
}

UUserWidget* AHW08PlayerController::GetHUDWidget() const
{
	return HUDWidgetInstance;
}

void AHW08PlayerController::ShowMainMenu(bool bIsRestart)
{
    bIsEnd = bIsRestart;

    if(HUDWidgetInstance)
    {
        HUDWidgetInstance->RemoveFromParent();
        HUDWidgetInstance = nullptr;
    }

    if(MainMenuWidgetInstance)
    {
        MainMenuWidgetInstance->RemoveFromParent();
        MainMenuWidgetInstance = nullptr;
    }

    if(MainMenuWidgetClass)
    {
        MainMenuWidgetInstance = CreateWidget<UUserWidget>(this, MainMenuWidgetClass);
        if(MainMenuWidgetInstance)
        {
            MainMenuWidgetInstance->AddToViewport();

            bShowMouseCursor = true;
            SetInputMode(FInputModeUIOnly());
        }

        if(UTextBlock* StartButtonText =
           Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName(TEXT("StartButtonText"))))
        {
            if(bIsRestart)
            {
                StartButtonText->SetText(FText::FromString(TEXT("Restart")));
            }
            else
            {
                StartButtonText->SetText(FText::FromString(TEXT("Start")));
            }
        }

        if(UTextBlock* EndButtonText =
           Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName(TEXT("EndButtonText"))))
        {
            if(bIsRestart)
            {
                EndButtonText->SetText(FText::FromString(TEXT("Return MainMenu")));
            }
            else
            {
                EndButtonText->SetText(FText::FromString(TEXT("Exit")));
            }
        }

        if(bIsRestart)
        {
            UFunction* PlayAnimFunc = MainMenuWidgetInstance->FindFunction(FName("PlayGameOverAnim"));
            if(PlayAnimFunc)
            {
                MainMenuWidgetInstance->ProcessEvent(PlayAnimFunc, nullptr);
            }

            if(UTextBlock* TotalScoreText =
               Cast<UTextBlock>(MainMenuWidgetInstance->GetWidgetFromName("TotalScoreText")))
            {
                if(UHW08GameInstance* HW08GameInstance =
                   Cast<UHW08GameInstance>(UGameplayStatics::GetGameInstance(this)))
                {
                    TotalScoreText->SetText(FText::FromString(
                        FString::Printf(TEXT("Total Score: %d"), HW08GameInstance->TotalScore)
                    ));
                }
            }
        }
    }
}

void AHW08PlayerController::ShowGameHUD()
{
    if(HUDWidgetInstance)
    {
        HUDWidgetInstance->RemoveFromParent();
        HUDWidgetInstance = nullptr;
    }

    if(MainMenuWidgetInstance)
    {
        MainMenuWidgetInstance->RemoveFromParent();
        MainMenuWidgetInstance = nullptr;
    }

    if(HUDWidgetClass)
    {
        HUDWidgetInstance = CreateWidget<UUserWidget>(this, HUDWidgetClass);
        if(HUDWidgetInstance)
        {
            HUDWidgetInstance->AddToViewport();

            bShowMouseCursor = false;
            SetInputMode(FInputModeGameOnly());
        }

        AHW08GameState* HW08GameState =
            GetWorld() ? GetWorld()->GetGameState<AHW08GameState>() : nullptr;

        if(HW08GameState)
        {
            HW08GameState->UpdateHUD();
        }
    }
}

void AHW08PlayerController::StartGame()
{
    if(UHW08GameInstance* HW08GameInstance =
       Cast<UHW08GameInstance>(UGameplayStatics::GetGameInstance(this)))
    {
        HW08GameInstance->CurrentLevelIndex = 0;
        HW08GameInstance->TotalScore = 0;
    }

    UGameplayStatics::OpenLevel(GetWorld(), FName("BasicLevel"));
    SetPause(false);
}