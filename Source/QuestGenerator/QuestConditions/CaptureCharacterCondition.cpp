// Copyright 2022 Jan Plähn. All Rights Reserved.

#include "CaptureCharacterCondition.h"
#include "AesirProceduralQuestBPLibrary.h"

bool UCaptureCharacterCondition::IsResolved_Implementation(const UObject* WorldContextObject) const
{
	return bInvertCondition; //Todo: Need to check world state here instead
}

FString UCaptureCharacterCondition::GetPropertyInfo_Implementation() const
{
	FString BaseString = Super::GetPropertyInfo_Implementation();
	BaseString.Append(FString::Printf(TEXT("Character: %s; "), *CharacterName.ToString()));	
	return BaseString;
}

uint32 UCaptureCharacterCondition::GenerateId() const
{
	return HashCombine(GetTypeHash(GetClass()), TextKeyUtil::HashString(CharacterName.ToString()));
}

void UCaptureCharacterCondition::InjectParameters(const TArray<UQuestParameter*>& Parameters)
{	
	UAesirProceduralQuestBPLibrary::InjectNameParameter(CharacterName, Parameters);
	Super::InjectParameters(Parameters);
}
