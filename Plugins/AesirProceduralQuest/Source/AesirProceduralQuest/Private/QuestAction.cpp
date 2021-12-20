// Copyright 2022 Jan Plähn. All Rights Reserved.

#include "QuestAction.h"

bool UQuestAction::IsAvailable(const UObject* WorldContextObject) const
{
	for (UQuestCondition* Condition : PreConditions)
	{
		if (!Condition->IsResolved(WorldContextObject))
		{
			return false;
		}
	}
	return true;
}

bool UQuestAction::SimulateIsAvailable(const UObject* WorldContextObject, TMap<uint32, bool> SimulatedConditionResolutions) const
{
	for (UQuestCondition* Condition : PreConditions)
	{
		bool* FoundResolution = SimulatedConditionResolutions.Find(Condition->GetId());

		if (FoundResolution != nullptr)
		{
			if (!Condition->SimulateIsResolved(WorldContextObject, *FoundResolution))
			{
				return false;
			}
		}
		else
		{
			if (!Condition->IsResolved(WorldContextObject))
			{
				return false;
			}
		}
	}
	return true;
}

bool UQuestAction::IsResolved(const UObject* WorldContextObject) const
{
	for (UQuestCondition* Condition : PostConditions)
	{
		if (!Condition->IsResolved(WorldContextObject))
		{
			return false;
		}
	}
	return true;
}

const TArray<UQuestCondition*>& UQuestAction::GetPreConditions() const
{
	return PreConditions;
}

const TArray<UQuestCondition*>& UQuestAction::GetPostConditions() const
{
	return PostConditions;
}
