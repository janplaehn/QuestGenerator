// Copyright 2022 Jan Plähn. All Rights Reserved.


#include "HasItemCondition.h"

bool UHasItemCondition::IsResolved_Implementation(const UObject* WorldContextObject) const
{
	return true;
}

bool UHasItemCondition::SimulateIsResolved_Implementation(const UObject* WorldContextObject, TArray<UQuestCondition*>& SimulatedPostConditions) const
{
	return true;
}

FString UHasItemCondition::GetPropertyInfo_Implementation() const
{
	FString BaseString = Super::GetPropertyInfo_Implementation();
	BaseString.Append(FString::Printf(TEXT("Item: %s; "), *ItemName.ToString()));	
	return BaseString;
}
