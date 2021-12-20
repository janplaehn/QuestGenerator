// Copyright 2022 Jan Plähn. All Rights Reserved.

#include "QuestCondition.h"

bool UQuestCondition::IsResolved_Implementation(const UObject* WorldContextObject) const
{
	return false;
}

FString UQuestCondition::GetPropertyInfo_Implementation() const
{
	return FString::Printf(TEXT("bInvertCondition: %s; "), bInvertCondition ? TEXT("true") : TEXT("false"));
}

void UQuestCondition::Init()
{
	ConditionId = GenerateId();
}

bool UQuestCondition::SimulateIsAvailable(const bool bWasPreviouslyResolved) const
{
	return !bMatchWithConditionHistory || bWasPreviouslyResolved != bInvertCondition;
}

uint32 UQuestCondition::GetId() const
{
	return ConditionId;
}

uint32 UQuestCondition::GenerateId() const
{
	return GetTypeHash(GetClass());
}
