// Copyright 2022 Jan Plähn. All Rights Reserved.

#include "FindCharacterCondition.h"

#include "AesirProceduralQuestBPLibrary.h"

bool UFindCharacterCondition::IsResolved_Implementation(const UObject* WorldContextObject) const
{
	return bInvertCondition; //Todo: Need to check world state here instead
}

FString UFindCharacterCondition::GetPropertyInfo_Implementation() const
{
	FString BaseString = Super::GetPropertyInfo_Implementation();
	BaseString.Append(FString::Printf(TEXT("Character: %s; "), *CharacterName.ToString()));	
	return BaseString;
}

uint32 UFindCharacterCondition::GenerateId() const
{
	return GetTypeHash(GetClass());
}

uint32 UFindCharacterCondition::GenerateStateId() const
{
	return TextKeyUtil::HashString(CharacterName.ToString());
}

EConditionType UFindCharacterCondition::GetConditionType_Implementation() const
{
	return EConditionType::State;
}

void UFindCharacterCondition::InjectParameters(const TArray<UQuestParameter*>& Parameters)
{	
	UAesirProceduralQuestBPLibrary::InjectNameParameter(CharacterName, Parameters);
	Super::InjectParameters(Parameters);
}
