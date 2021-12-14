// Copyright 2022 Jan Plähn. All Rights Reserved.

#include "FindCharacterCondition.h"

bool UFindCharacterCondition::IsResolved_Implementation(const UObject* WorldContextObject) const
{
	return true;
}

bool UFindCharacterCondition::SimulateIsResolved_Implementation(const UObject* WorldContextObject, TArray<UQuestCondition*>& SimulatedPostConditions) const
{
	bool bHasFoundCharacter = false; //Todo: Need to check world state here instead

	for (UQuestCondition* Condition : SimulatedPostConditions)
	{
		UFindCharacterCondition* FindCharacterCondition = Cast<UFindCharacterCondition>(Condition);
		if (!IsValid(FindCharacterCondition))
		{
			continue;
		}
		if (FindCharacterCondition->CharacterName != CharacterName)
		{
			continue;
		}
		bHasFoundCharacter = bHasFoundCharacter || !FindCharacterCondition->bInvertCondition;
	}
	return bHasFoundCharacter != bInvertCondition;
}

FString UFindCharacterCondition::GetPropertyInfo_Implementation() const
{
	FString BaseString = Super::GetPropertyInfo_Implementation();
	BaseString.Append(FString::Printf(TEXT("Character: %s; "), *CharacterName.ToString()));	
	return BaseString;
}
