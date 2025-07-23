#include "HealingItem.h"
#include "HW08Character.h"
AHealingItem::AHealingItem()
{
	HealAmount = 20.0f;
	ItemType = "Healing";
}

void AHealingItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);

	if(Activator && Activator->ActorHasTag("Player"))
	{
		if(AHW08Character* PlayerCharacter = Cast<AHW08Character>(Activator))
		{
			PlayerCharacter->AddHealth(HealAmount);
		}
		DestroyItem();
	}
}