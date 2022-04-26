// Copyright 2022 Jan Plähn. All Rights Reserved.

#include "UQuestParameter_PlayerLocationBased.h"
#include "QuestGenerator/QuestConditions/PlayerIsAtLocationCondition.h"
#include "QuestGenerator/WorldState/SimulatedWorldStateComponent.h"

bool UQuestParameter_PlayerLocationBased::FindPlayerLocation(const UQuest* Quest, FName& OutLocationName) const
{
	//Todo: Instead of looping over and over and over, probably it would make sense to just store this somewhere!
	
	const TArray<TWeakObjectPtr<UQuestAction>>& QuestActions = Quest->GetActions();
	for (int32 ActionIndex = QuestActions.Num() - 1; ActionIndex >= 0; ActionIndex--)
	{
		const TArray<UQuestCondition*>& PostConditions = QuestActions[ActionIndex]->GetPostConditions();
		for (const UQuestCondition* Condition  : PostConditions)
		{
			if (const UPlayerIsAtLocationCondition* PlayerIsAtLocationCondition = Cast<UPlayerIsAtLocationCondition>(Condition))
			{
				OutLocationName = PlayerIsAtLocationCondition->GetLocationName();
				return true;
			}
		}
	}

	return false;
}
