// Fill out your copyright notice in the Description page of Project Settings.

#include "QuestAction.h"

bool UQuestAction::IsAvailable() const
{
	for (UQuestCondition* Condition : PreConditions)
	{
		if (!Condition->IsResolved())
		{
			return false;
		}
	}
	return true;
}

bool UQuestAction::IsResolved() const
{
	for (UQuestCondition* Condition : PostConditions)
	{
		if (!Condition->IsResolved())
		{
			return false;
		}
	}
	return true;
}
