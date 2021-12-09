﻿// Copyright 2022 Jan Plähn. All Rights Reserved.

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
