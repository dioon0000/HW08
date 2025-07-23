#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "HW08Character.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UWidgetComponent;
struct FInputActionValue;

UCLASS()
class HW08_API AHW08Character : public ACharacter
{
	GENERATED_BODY()

public:
	AHW08Character();

	// Variable
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera")
	USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera")
	UCameraComponent* CameraComp;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	UWidgetComponent* OverheadWidget;

	// Function
	UFUNCTION(BlueprintPure, Category="Health")
	float GetHealth() const;
	UFUNCTION(BlueprintCallable, Category="Health")
	void AddHealth(float Amount);
	UFUNCTION(BlueprintCallable, Category="Movement")
	void ApplySpeedModifier(float Multiplier, float Duration);
	UFUNCTION(BlueprintCallable, Category="Movement")
	void ApplyReverseControls(float Duration);

protected:
	// Variable
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Health")
	float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Health")
	float Health;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement|Base")
	float BaseWalkSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement|Base")
	float BaseSprintSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	float CurrentSpeedMultiplier;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bIsSprinting;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
	bool bAreControlsReversed;

	FTimerHandle SpeedModifierTimerHandle;
	FTimerHandle ReverseControlsTimerHandle;

	// Function
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(
		float DamageAmount, // 데미지양
		struct FDamageEvent const& DamageEvent, // 데미지를 받은 어떤 유형
		AController* EventInstigator, // 데미지를 누가 입혔는지 ex. 지뢰를 심은 사람?(지뢰는 아래 AActor* DamageCauser에 해당)
		AActor* DamageCauser // 데미지를 일으킨 오브젝트
	) override;

	UFUNCTION()
	void Move(const FInputActionValue& value);
	UFUNCTION()
	void StartJump(const FInputActionValue& value);
	UFUNCTION()
	void StopJump(const FInputActionValue& value);
	UFUNCTION()
	void Look(const FInputActionValue& value);
	UFUNCTION()
	void StartSprint(const FInputActionValue& value);
	UFUNCTION()
	void StopSprint(const FInputActionValue& value);

	void UpdateCharacterSpeed();
	void RestoreDefaultSpeedModifier();
	void RestoreDefaultControls();
	void OnDeath();
	void UpdateOverheadHP();
};