#include "ReverseControlItem.h"
#include "HW08Character.h"

AReverseControlItem::AReverseControlItem()
{
	ReverseDuration = 5.0f;
}

void AReverseControlItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);

	AHW08Character* PlayerCharacter = Cast<AHW08Character>(Activator);
	if(PlayerCharacter)
	{
		PlayerCharacter->ApplyReverseControls(ReverseDuration);
	}

	DestroyItem();
}