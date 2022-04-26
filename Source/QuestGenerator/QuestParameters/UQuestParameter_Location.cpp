// Copyright 2022 Jan Plähn. All Rights Reserved.

#include "UQuestParameter_Location.h"

bool UQuestParameter_Location::GenerateValueFromWorldState(const UQuest* Quest, FName& OutValue) const
{
	return FindPlayerLocation(Quest, OutValue);
}
