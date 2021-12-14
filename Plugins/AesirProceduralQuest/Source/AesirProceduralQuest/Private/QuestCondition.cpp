// Copyright 2022 Jan Plähn. All Rights Reserved.

#include "QuestCondition.h"

bool UQuestCondition::IsResolved_Implementation() const
{
	return false;
}

bool UQuestCondition::SimulateIsResolved_Implementation(TArray<UQuestCondition*>& SimulatedPostConditions) const
{
	return false;
}
