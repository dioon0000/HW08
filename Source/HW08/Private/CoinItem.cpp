#include "CoinItem.h"
#include "Engine/World.h"
#include "HW08GameState.h"

ACoinItem::ACoinItem()
{
	PointValue = 0;
	ItemType = "DefaultCoin";
}

void ACoinItem::ActivateItem(AActor* Activator)
{
	
	Super::ActivateItem(Activator);

	if(Activator && Activator->ActorHasTag("Player"))
	{
		if(UWorld* World = GetWorld())
		{
			if(AHW08GameState* GameState = World->GetGameState<AHW08GameState>())
			{
				GameState->AddScore(PointValue);
				GameState->OnCoinCollected();
			}
		}
		DestroyItem();
	}
	
}