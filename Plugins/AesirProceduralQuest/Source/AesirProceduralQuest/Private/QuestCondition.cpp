// Copyright 2022 Jan Plähn. All Rights Reserved.

#include "QuestCondition.h"

#include "UObject/PropertyAccessUtil.h"

bool UQuestCondition::IsResolved_Implementation(const UObject* WorldContextObject) const
{
	return false;
}

bool UQuestCondition::SimulateIsResolved_Implementation(const UObject* WorldContextObject, TArray<UQuestCondition*>& SimulatedPostConditions) const
{
	return false;
}

FString UQuestCondition::GetPropertyInfo_Implementation() const
{
	return FString::Printf(TEXT("bInvertCondition: %s; "), bInvertCondition ? TEXT("true") : TEXT("false"));
}
