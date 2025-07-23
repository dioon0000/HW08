#include "SlowingItem.h"
#include "HW08Character.h"

ASlowingItem::ASlowingItem()
{
	SlowDuration = 5.0f;
	DecreseSpeedMultiplier = 0.5f;
}

// 아이템을 획득하면 PlayerCharacter의 이동 속도를 50% 감소시킴
// NormalSpeed * 0.5
// 해당 과정을 5초간 진행 (중첩 가능)
void ASlowingItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);

	AHW08Character* PlayerCharacter = Cast<AHW08Character>(Activator);
	if(PlayerCharacter)
	{
		PlayerCharacter->ApplySpeedModifier(DecreseSpeedMultiplier, SlowDuration);
	}

	DestroyItem();
}