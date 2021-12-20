// Copyright 2022 Jan Plähn. All Rights Reserved.

#include "FindCharacterCondition.h"

bool UFindCharacterCondition::IsResolved_Implementation(const UObject* WorldContextObject) const
{
	return bInvertCondition;
}

bool UFindCharacterCondition::SimulateIsResolved_Implementation(const UObject* WorldContextObject, bool bWasPreviouslyResolved) const
{
	return bWasPreviouslyResolved != bInvertCondition;
}

FString UFindCharacterCondition::GetPropertyInfo_Implementation() const
{
	FString BaseString = Super::GetPropertyInfo_Implementation();
	BaseString.Append(FString::Printf(TEXT("Character: %s; "), *CharacterName.ToString()));	
	return BaseString;
}

uint32 UFindCharacterCondition::GetId() const
{
	return HashCombine(GetTypeHash(GetClass()), TextKeyUtil::HashString(CharacterName.ToString()));
}
