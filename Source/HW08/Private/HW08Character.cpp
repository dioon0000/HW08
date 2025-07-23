#include "HW08Character.h"
#include "HW08PlayerController.h"
#include "HW08GameState.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/TextBlock.h"

AHW08Character::AHW08Character()
{
	PrimaryActorTick.bCanEverTick = false;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->TargetArmLength = 300.0f;
	SpringArmComp->bUsePawnControlRotation = true; // Pawn 회전에 따라 스프링 암도 같이 회전

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false; // 이미 스프링암에서 회전 처리를 하므로 카메라는 false

	OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	OverheadWidget->SetupAttachment(GetMesh());
	OverheadWidget->SetWidgetSpace(EWidgetSpace::Screen);

	MaxHealth = 100.0f;
	Health = MaxHealth;

	BaseWalkSpeed = 600.0f;
	BaseSprintSpeed = 900.0f;
	CurrentSpeedMultiplier = 1.5f;
	bIsSprinting = false;
	bAreControlsReversed = false;

	GetCharacterMovement()->MaxWalkSpeed = BaseWalkSpeed;
}

void AHW08Character::BeginPlay()
{
	Super::BeginPlay();

	UpdateOverheadHP();
}

float AHW08Character::GetHealth() const
{
	return Health;
}

void AHW08Character::AddHealth(float Amount)
{
	Health = FMath::Clamp(Health + Amount, 0.0f, MaxHealth);
	UpdateOverheadHP();
}

void AHW08Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if(UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if(AHW08PlayerController* PlayerController = Cast<AHW08PlayerController>(GetController()))
		{
			if(PlayerController->MoveAction)
			{
				EnhancedInput->BindAction(
					PlayerController->MoveAction,
					ETriggerEvent::Triggered,
					this,
					&AHW08Character::Move
				);
			}

			if(PlayerController->JumpAction)
			{
				EnhancedInput->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Triggered,
					this,
					&AHW08Character::StartJump
				);
			}

			if(PlayerController->JumpAction)
			{
				EnhancedInput->BindAction(
					PlayerController->JumpAction,
					ETriggerEvent::Completed,
					this,
					&AHW08Character::StopJump
				);
			}

			if(PlayerController->LookAction)
			{
				EnhancedInput->BindAction(
					PlayerController->LookAction,
					ETriggerEvent::Triggered,
					this,
					&AHW08Character::Look
				);
			}

			if(PlayerController->SprintAction)
			{
				EnhancedInput->BindAction(
					PlayerController->SprintAction,
					ETriggerEvent::Triggered,
					this,
					&AHW08Character::StartSprint
				);
			}

			if(PlayerController->SprintAction)
			{
				EnhancedInput->BindAction(
					PlayerController->SprintAction,
					ETriggerEvent::Completed,
					this,
					&AHW08Character::StopSprint
				);
			}
		}
	}
}

void AHW08Character::Move(const FInputActionValue& value)
{
	if(!Controller) return;

	const FVector2D MoveInput = value.Get<FVector2D>();
	const FVector2D RealMoveInput = bAreControlsReversed ? -MoveInput : MoveInput;

	if(!FMath::IsNearlyZero(RealMoveInput.X))
	{
		AddMovementInput(GetActorForwardVector(), RealMoveInput.X);
	}

	if(!FMath::IsNearlyZero(RealMoveInput.Y))
	{
		AddMovementInput(GetActorRightVector(), RealMoveInput.Y);
	}
}

void AHW08Character::StartJump(const FInputActionValue& value)
{
	if(value.Get<bool>())
	{
		Jump();
	}
}

void AHW08Character::StopJump(const FInputActionValue& value)
{
	if(!value.Get<bool>())
	{
		StopJumping();
	}
}

void AHW08Character::Look(const FInputActionValue& value)
{
	FVector2D LookInput = value.Get<FVector2D>();

	AddControllerYawInput(LookInput.X);
	AddControllerPitchInput(LookInput.Y);
}

void AHW08Character::StartSprint(const FInputActionValue& value)
{
	if(GetCharacterMovement())
	{
		bIsSprinting = true;
		UpdateCharacterSpeed();
	}
}
void AHW08Character::StopSprint(const FInputActionValue& value)
{
	if(GetCharacterMovement())
	{
		bIsSprinting = false;
		UpdateCharacterSpeed();
	}
}

float AHW08Character::TakeDamage(
	float DamageAmount,
	struct FDamageEvent const& DamageEvent,
	AController* EventInstigator,
	AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Health = FMath::Clamp(Health - DamageAmount, 0.0f, MaxHealth);
	UpdateOverheadHP();

	if(Health <= 0.0f)
	{
		OnDeath();
	}
	return ActualDamage;
}

void AHW08Character::UpdateCharacterSpeed()
{
	float CurrentBaseSpeed = bIsSprinting ? BaseSprintSpeed : BaseWalkSpeed;

	GetCharacterMovement()->MaxWalkSpeed = CurrentBaseSpeed * CurrentSpeedMultiplier;
}

void AHW08Character::ApplySpeedModifier(float Multiplier, float Duration)
{
	CurrentSpeedMultiplier = Multiplier;
	UpdateCharacterSpeed();

	GetWorld()->GetTimerManager().ClearTimer(SpeedModifierTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(
		SpeedModifierTimerHandle,
		this,
		&AHW08Character::RestoreDefaultSpeedModifier,
		Duration,
		false
	);
}

void AHW08Character::ApplyReverseControls(float Duration)
{
	bAreControlsReversed = true;

	GetWorld()->GetTimerManager().ClearTimer(ReverseControlsTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(
		ReverseControlsTimerHandle,
		this,
		&AHW08Character::RestoreDefaultControls,
		Duration,
		false
	);
}

void AHW08Character::RestoreDefaultSpeedModifier()
{
	CurrentSpeedMultiplier = 1.0f;
	UpdateCharacterSpeed();
}

void AHW08Character::RestoreDefaultControls()
{
	bAreControlsReversed = false;
}

void AHW08Character::OnDeath()
{
	// 게임 종료 로직
	AHW08GameState* HW08GameState = GetWorld() ? GetWorld()->GetGameState<AHW08GameState>() : nullptr;

	if(HW08GameState)
	{
		HW08GameState->OnGameOver();
	}
}

void AHW08Character::UpdateOverheadHP()
{
	if(!OverheadWidget) return;

	UUserWidget* OverheadWidgetInstance = OverheadWidget->GetUserWidgetObject();
	if(!OverheadWidgetInstance) return;

	if(UTextBlock* HPText = Cast<UTextBlock>(OverheadWidgetInstance->GetWidgetFromName(TEXT("OverHeadHP"))))
	{
		HPText->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), Health, MaxHealth)));
	}
}