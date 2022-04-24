// Copyright 2022 Jan Plähn. All Rights Reserved.

#include "IsAliveCondition.h"
#include "AesirProceduralQuestBPLibrary.h"

bool UIsAliveCondition::IsResolved_Implementation(const UObject* WorldContextObject) const
{
	return !bInvertCondition; //Todo: Need to check world state here instead
}

FString UIsAliveCondition::GetPropertyInfo_Implementation() const
{
	FString BaseString = Super::GetPropertyInfo_Implementation();
	BaseString.Append(FString::Printf(TEXT("Character: %s; "), *CharacterName.ToString()));	
	return BaseString;
}

uint32 UIsAliveCondition::GenerateId() const
{
	const uint32 TypeHash = GetTypeHash(GetClass());
	const uint32 CharacterNameHash = TextKeyUtil::HashString(CharacterName.ToString());
	const uint32 CombinedHash = HashCombine(TypeHash, CharacterNameHash);
	return CombinedHash;
}

void UIsAliveCondition::InjectParameters(const TMap<FName, FName>& ParameterValues)
{
	UAesirProceduralQuestBPLibrary::InjectNameParameter(CharacterName, ParameterValues);
	Super::InjectParameters(ParameterValues);
}

TSet<FName> UIsAliveCondition::GetParameters() const
{
	return TSet<FName>
	{
		CharacterName
	};
}
