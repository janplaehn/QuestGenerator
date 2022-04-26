// Copyright 2022 Jan Plähn. All Rights Reserved.

#include "UQuestParameter_PlayerLocationBased.h"

#include "QuestGenerator/QuestConditions/HasItemCondition.h"
#include "QuestGenerator/QuestConditions/PlayerIsAtLocationCondition.h"

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
				if (!PlayerIsAtLocationCondition->bInvertCondition)
				{
					OutLocationName = PlayerIsAtLocationCondition->GetLocationName();
					return true;
				}
			}
		}
	}

	return false;
}

TArray<FName> UQuestParameter_PlayerLocationBased::FindReceivedItems(const UQuest* Quest) const
{
	TArray<FName> Items;
	
	const TArray<TWeakObjectPtr<UQuestAction>>& QuestActions = Quest->GetActions();
	for (int32 ActionIndex = QuestActions.Num() - 1; ActionIndex >= 0; ActionIndex--)
	{
		const TArray<UQuestCondition*>& PostConditions = QuestActions[ActionIndex]->GetPostConditions();
		for (const UQuestCondition* Condition  : PostConditions)
		{
			if (const UHasItemCondition* HasItemCondition = Cast<UHasItemCondition>(Condition))
			{
				if (!HasItemCondition->bInvertCondition)
				{
					Items.Add(HasItemCondition->GetItemName());
				}
			}
		}
	}

	return Items;
}
