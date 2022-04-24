// Copyright 2022 Jan Plähn. All Rights Reserved.

#include "ItemIsAtLocationCondition.h"
#include "AesirProceduralQuestBPLibrary.h"
#include "QuestGenerator/WorldState/SimulatedWorldStateComponent.h"

bool UItemIsAtLocationCondition::IsResolved_Implementation(const UObject* WorldContextObject) const
{
	USimulatedWorldStateComponent* WorldStateComponent = USimulatedWorldStateComponent::TryFindWorldStateComponent(WorldContextObject);
	if (!IsValid(WorldStateComponent))
	{
		return bInvertCondition;
	}
	FName* FoundItemName = WorldStateComponent->ItemIsAtLocationMap.Find(ItemName);
	if (FoundItemName == nullptr)
	{
		return bInvertCondition;
	}
	return (TextKeyUtil::HashString(FoundItemName->ToString()) == GetStateId()) != bInvertCondition;
}

FString UItemIsAtLocationCondition::GetPropertyInfo_Implementation() const
{
	FString BaseString = Super::GetPropertyInfo_Implementation();
	BaseString.Append(FString::Printf(TEXT("Location %s; "), *LocationName.ToString()));	
	return BaseString;
}

uint32 UItemIsAtLocationCondition::GenerateId() const
{
	const uint32 TypeHash = GetTypeHash(GetClass());
	const uint32 CharacterNameHash = TextKeyUtil::HashString(ItemName.ToString());
	const uint32 CombinedHash = HashCombine(TypeHash, CharacterNameHash);
	return CombinedHash;
}

uint32 UItemIsAtLocationCondition::GenerateStateId() const
{
	return TextKeyUtil::HashString(LocationName.ToString());
}

EConditionType UItemIsAtLocationCondition::GetConditionType_Implementation() const
{
	return EConditionType::State;
}

void UItemIsAtLocationCondition::InjectParameters(const TMap<FName, FName>& ParameterValues)
{
	UAesirProceduralQuestBPLibrary::InjectNameParameter(LocationName, ParameterValues);
	UAesirProceduralQuestBPLibrary::InjectNameParameter(ItemName, ParameterValues);
	Super::InjectParameters(ParameterValues);
}

TSet<FName> UItemIsAtLocationCondition::GetParameters() const
{
	return TSet<FName>
	{
		ItemName,
		LocationName
	};
}
