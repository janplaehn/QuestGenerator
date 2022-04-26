// Copyright 2022 Jan Plähn. All Rights Reserved.

#include "UQuestParameter_Item.h"
#include "QuestGenerator/WorldState/SimulatedWorldStateComponent.h"

bool UQuestParameter_Item::GenerateValueFromWorldState(const UQuest* Quest, FName& OutValue) const
{
	FName PlayerLocation;
	if (!FindPlayerLocation(Quest, PlayerLocation))
	{
		return false;
	}

	const USimulatedWorldStateComponent* WorldStateComponent = USimulatedWorldStateComponent::TryFindWorldStateComponent(Quest);

	TArray<FName> PossibleItems;

	//Todo: This below is horrible and slow thanks to how we built our SimulatedWorldStateComponent!
	{
		TArray<FName> CharactersAtLocation;
		
		for (const auto& Kvp : WorldStateComponent->ItemIsAtLocationMap)
			if (Kvp.Value == PlayerLocation)
				PossibleItems.Add(Kvp.Key);
	
		for (const auto& Kvp : WorldStateComponent->CharacterIsAtLocationMap)
			if (Kvp.Value == PlayerLocation)
				CharactersAtLocation.Add(Kvp.Key);

		for (const auto& Kvp : WorldStateComponent->CharacterHasItemMap)
			if (CharactersAtLocation.Contains(Kvp.Key))
				PossibleItems.Append(Kvp.Value.Items);
	}

	if (!PossibleItems.Num())
	{
		return false;
	}

	const uint32 RandomItemIndex = FMath::RandRange(0, PossibleItems.Num() - 1);
	OutValue = PossibleItems[RandomItemIndex];
	return true;
}
