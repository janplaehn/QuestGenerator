// Copyright 2022 Jan Plähn. All Rights Reserved.

#include "PlayerIsAtLocationCondition.h"
#include "AesirProceduralQuestBPLibrary.h"

bool UPlayerIsAtLocationCondition::IsResolved_Implementation(const UObject* WorldContextObject) const
{
	//Todo: check if world state provides us with item
	return bInvertCondition;
}

FString UPlayerIsAtLocationCondition::GetPropertyInfo_Implementation() const
{
	FString BaseString = Super::GetPropertyInfo_Implementation();
	BaseString.Append(FString::Printf(TEXT("Location %s; "), *LocationName.ToString()));	
	return BaseString;
}

uint32 UPlayerIsAtLocationCondition::GenerateId() const
{
	return GetTypeHash(GetClass());
}

uint32 UPlayerIsAtLocationCondition::GenerateStateId() const
{
	return TextKeyUtil::HashString(LocationName.ToString());
}

EConditionType UPlayerIsAtLocationCondition::GetConditionType_Implementation() const
{
	return EConditionType::State;
}

void UPlayerIsAtLocationCondition::InjectParameters(const TMap<FName, FName>& ParameterValues)
{
	UAesirProceduralQuestBPLibrary::InjectNameParameter(LocationName, ParameterValues);
	Super::InjectParameters(ParameterValues);
}

TSet<FName> UPlayerIsAtLocationCondition::GetParameters() const
{
	return TSet<FName>
	{
		LocationName
	};
}
