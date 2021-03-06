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

bool UQuestCondition::SimulateIsAvailable(const uint32 Resolution) const
{
	bool bWasPreviouslyResolved;
	
	switch (GetConditionType())
	{
		default:
		case EConditionType::Boolean:
			{
				bWasPreviouslyResolved = Resolution > 0; 
				break;
			}
		case EConditionType::State:
			{
				bWasPreviouslyResolved = Resolution == GetStateId();
				break;
			}
	}
	return !bMatchWithConditionHistory || bWasPreviouslyResolved != bInvertCondition;
}

uint32 UQuestCondition::GetId() const
{
	return ConditionId;
}

uint32 UQuestCondition::GetStateId() const
{
	return StateId;
}

EConditionType UQuestCondition::GetConditionType_Implementation() const
{
	return EConditionType::Boolean;
}

void UQuestCondition::Initialize(const TMap<FName, FName>& ParameterValues)
{
	InjectParameters(ParameterValues);
	ConditionId = GenerateId();
	StateId = GenerateStateId();
}

uint32 UQuestCondition::GenerateId() const
{
	return GetTypeHash(GetClass());
}

uint32 UQuestCondition::GenerateStateId() const
{
	return 0;
}
