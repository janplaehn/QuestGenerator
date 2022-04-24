// Copyright 2022 Jan Plähn. All Rights Reserved.

#include "HasItemCondition.h"
#include "AesirProceduralQuestBPLibrary.h"

bool UHasItemCondition::IsResolved_Implementation(const UObject* WorldContextObject) const
{
	//Todo: check if world state provides us with item
	return bInvertCondition;
}

FString UHasItemCondition::GetPropertyInfo_Implementation() const
{
	FString BaseString = Super::GetPropertyInfo_Implementation();
	BaseString.Append(FString::Printf(TEXT("Item: %s; "), *ItemName.ToString()));	
	return BaseString;
}

uint32 UHasItemCondition::GenerateId() const
{
	const uint32 TypeHash = GetTypeHash(GetClass());
	const uint32 ItemNameHash = TextKeyUtil::HashString(ItemName.ToString());
	const uint32 CombinedHash = HashCombine(TypeHash, ItemNameHash);
	return CombinedHash;
}

void UHasItemCondition::InjectParameters(const TMap<FName, FName>& ParameterValues)
{
	UAesirProceduralQuestBPLibrary::InjectNameParameter(ItemName, ParameterValues);
	Super::InjectParameters(ParameterValues);
}

TSet<FName> UHasItemCondition::GetParameters() const
{
	return TSet<FName>
	{
		ItemName
	};
}
