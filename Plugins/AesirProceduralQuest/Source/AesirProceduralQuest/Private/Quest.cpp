// Copyright 2022 Jan Plähn. All Rights Reserved.

#include "Quest.h"

bool UQuest::IsAvailable(const UObject* WorldContextObject) const
{
	if (Actions.Num() == 0)
	{
		return true;
	}
	return Actions[0]->IsAvailable(WorldContextObject);
}

bool UQuest::IsResolved(const UObject* WorldContextObject) const
{
	for (const TSoftObjectPtr<UQuestAction> Action : Actions)
	{
		if (!Action->IsResolved(WorldContextObject))
		{
			return false;
		}
	}
	return true;
}

void UQuest::AddQuestAction(const UQuestAction* NewAction)
{
	Actions.Add(NewAction);
}

const TArray<const UQuestAction*>& UQuest::GetActions() const
{
	return Actions;
}
