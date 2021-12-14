// Copyright 2022 Jan Plähn. All Rights Reserved.


#include "IsAliveCondition.h"

bool UIsAliveCondition::IsResolved_Implementation(const UObject* WorldContextObject) const
{
	return true; //Todo: Need to check world state here instead
}

bool UIsAliveCondition::SimulateIsResolved_Implementation(const UObject* WorldContextObject, TArray<UQuestCondition*>& SimulatedPostConditions) const
{

	bool bIsAlive = true; //Todo: Need to check world state here instead

	for (UQuestCondition* Condition : SimulatedPostConditions)
	{
		UIsAliveCondition* IsAliveCondition = Cast<UIsAliveCondition>(Condition);
		if (!IsValid(IsAliveCondition))
		{
			continue;
		}
		if (IsAliveCondition->CharacterName != CharacterName)
		{
			continue;
		}
		bIsAlive = !IsAliveCondition->bInvertCondition;
	}
	return bIsAlive != bInvertCondition;
}

FString UIsAliveCondition::GetPropertyInfo_Implementation() const
{
	FString BaseString = Super::GetPropertyInfo_Implementation();
	BaseString.Append(FString::Printf(TEXT("Character: %s; "), *CharacterName.ToString()));	
	return BaseString;
}
