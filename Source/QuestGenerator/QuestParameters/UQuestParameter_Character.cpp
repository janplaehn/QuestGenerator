// Copyright 2022 Jan Plähn. All Rights Reserved.

#include "UQuestParameter_Character.h"
#include "QuestGenerator/WorldState/SimulatedWorldStateComponent.h"

bool UQuestParameter_Character::GenerateValueFromWorldState(const UQuest* Quest, FName& OutValue) const
{
	FName PlayerLocation;
	if (!FindPlayerLocation(Quest, PlayerLocation))
	{
		return false;
	}

	const USimulatedWorldStateComponent* WorldStateComponent = USimulatedWorldStateComponent::TryFindWorldStateComponent(Quest);

	TArray<FName> PossibleCharacters;

	//Todo: This below is horrible and slow thanks to how we built our SimulatedWorldStateComponent!
	{
		for (const auto& Kvp : WorldStateComponent->CharacterIsAtLocationMap)
			if (Kvp.Value == PlayerLocation)
				PossibleCharacters.Add(Kvp.Key);
	}

	if (!PossibleCharacters.Num())
	{
		return false;
	}

	const uint32 RandomItemIndex = FMath::RandRange(0, PossibleCharacters.Num() - 1);
	OutValue = PossibleCharacters[RandomItemIndex];
	return true;
}
