#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "HW08Character.h"
#include "SlowingItem.generated.h"

UCLASS()
class HW08_API ASlowingItem : public ABaseItem
{
	GENERATED_BODY()
	
public:
	ASlowingItem();

	virtual void ActivateItem(AActor* Activator) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item")
	float SlowDuration;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Item")
	float DecreseSpeedMultiplier;
};