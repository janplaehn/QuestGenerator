// Copyright 2022 Jan Plähn. All Rights Reserved.


#include "HasItemCondition.h"

bool UHasItemCondition::IsResolved_Implementation(const UObject* WorldContextObject) const
{
	//Todo: check if world state provides us with item
	return bInvertCondition;
}

bool UHasItemCondition::SimulateIsResolved_Implementation(const UObject* WorldContextObject, bool bWasPreviouslyResolved) const
{
	return bWasPreviouslyResolved != bInvertCondition;
}

FString UHasItemCondition::GetPropertyInfo_Implementation() const
{
	FString BaseString = Super::GetPropertyInfo_Implementation();
	BaseString.Append(FString::Printf(TEXT("Item: %s; "), *ItemName.ToString()));	
	return BaseString;
}

uint32 UHasItemCondition::GenerateId() const
{
	return HashCombine(GetTypeHash(GetClass()), TextKeyUtil::HashString(ItemName.ToString()));
}
