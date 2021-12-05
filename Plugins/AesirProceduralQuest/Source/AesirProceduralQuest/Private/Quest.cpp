// Fill out your copyright notice in the Description page of Project Settings.


#include "Quest.h"

bool UQuest::IsAvailable() const
{
	if (Actions.Num() == 0)
	{
		return true;
	}
	return Actions[0]->IsAvailable();
}

bool UQuest::IsResolved() const
{
	for (const UQuestAction* Action : Actions)
	{
		if (!Action->IsResolved())
		{
			return false;
		}
	}
	return true;
}
