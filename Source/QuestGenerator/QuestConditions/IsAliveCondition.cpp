// Copyright 2022 Jan Plähn. All Rights Reserved.


#include "IsAliveCondition.h"

bool UIsAliveCondition::IsResolved_Implementation(const UObject* WorldContextObject) const
{
	return true;
}

bool UIsAliveCondition::SimulateIsResolved_Implementation(const UObject* WorldContextObject, TArray<UQuestCondition*>& SimulatedPostConditions) const
{
	return true;
}

FString UIsAliveCondition::GetPropertyInfo_Implementation() const
{
	FString BaseString = Super::GetPropertyInfo_Implementation();
	BaseString.Append(FString::Printf(TEXT("Character: %s; "), *CharacterName.ToString()));	
	return BaseString;
}
