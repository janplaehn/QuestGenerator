// Fill out your copyright notice in the Description page of Project Settings.


#include "Quest.h"

bool UQuest::IsAvailable() const
{
	if (Actions.Num() == 0)
	{
		return true;
	}
	return Actions[0]->IsAvailable();

	//Todo: Instantiate and Initialize QuestActions when applicable or use the COD
}

bool UQuest::IsResolved() const
{
	for (const TSoftObjectPtr<UQuestAction> Action : Actions)
	{
		if (!Action->IsResolved())
		{
			return false;
		}
	}
	return true;

	//Todo: Instantiate and Initialize Conditions when applicable or use the COD
}

void UQuest::AddQuestAction(const TSoftObjectPtr<UQuestAction> NewAction)
{
	Actions.Add(NewAction);
}
