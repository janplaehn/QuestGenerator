// Copyright 2022 Jan Plähn. All Rights Reserved.

#include "CharacterIsAtLocationCondition.h"
#include "AesirProceduralQuestBPLibrary.h"
#include "QuestGenerator/WorldState/SimulatedWorldStateComponent.h"

bool UCharacterIsAtLocationCondition::IsResolved_Implementation(const UObject* WorldContextObject) const
{
	USimulatedWorldStateComponent* WorldStateComponent = USimulatedWorldStateComponent::TryFindWorldStateComponent(WorldContextObject);
	if (!IsValid(WorldStateComponent))
	{
		return bInvertCondition;
	}
	FName* FoundLocationName = WorldStateComponent->CharacterIsAtLocationMap.Find(CharacterName);
	if (FoundLocationName == nullptr)
	{
		return bInvertCondition;
	}
	return (TextKeyUtil::HashString(FoundLocationName->ToString()) == GetStateId()) != bInvertCondition;
}

FString UCharacterIsAtLocationCondition::GetPropertyInfo_Implementation() const
{
	FString BaseString = Super::GetPropertyInfo_Implementation();
	BaseString.Append(FString::Printf(TEXT("Location %s; "), *LocationName.ToString()));	
	return BaseString;
}

uint32 UCharacterIsAtLocationCondition::GenerateId() const
{
	const uint32 TypeHash = GetTypeHash(GetClass());
	const uint32 CharacterNameHash = TextKeyUtil::HashString(CharacterName.ToString());
	const uint32 CombinedHash = HashCombine(TypeHash, CharacterNameHash);
	return CombinedHash;
}

uint32 UCharacterIsAtLocationCondition::GenerateStateId() const
{
	return TextKeyUtil::HashString(LocationName.ToString());
}

EConditionType UCharacterIsAtLocationCondition::GetConditionType_Implementation() const
{
	return EConditionType::State;
}

void UCharacterIsAtLocationCondition::InjectParameters(const TArray<UQuestParameter*>& Parameters)
{
	UAesirProceduralQuestBPLibrary::InjectNameParameter(LocationName, Parameters);
	UAesirProceduralQuestBPLibrary::InjectNameParameter(CharacterName, Parameters);
	Super::InjectParameters(Parameters);
}
