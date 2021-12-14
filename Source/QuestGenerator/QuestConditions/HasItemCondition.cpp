// Copyright 2022 Jan Plähn. All Rights Reserved.


#include "HasItemCondition.h"

bool UHasItemCondition::IsResolved_Implementation(const UObject* WorldContextObject) const
{
	//Todo: check if world state provides us with item
	return true;
}

bool UHasItemCondition::SimulateIsResolved_Implementation(const UObject* WorldContextObject, TArray<UQuestCondition*>& SimulatedPostConditions) const
{
	bool bHasItem = false; //Todo: Need to check world state here instead

	for (UQuestCondition* Condition : SimulatedPostConditions)
	{
		UHasItemCondition* HasItemCondition = Cast<UHasItemCondition>(Condition);
		if (!IsValid(HasItemCondition))
		{
			continue;
		}
		if (HasItemCondition->ItemName != ItemName)
		{
			continue;
		}
		bHasItem = !HasItemCondition->bInvertCondition;
	}
	return bHasItem != bInvertCondition;
}

FString UHasItemCondition::GetPropertyInfo_Implementation() const
{
	FString BaseString = Super::GetPropertyInfo_Implementation();
	BaseString.Append(FString::Printf(TEXT("Item: %s; "), *ItemName.ToString()));	
	return BaseString;
}
