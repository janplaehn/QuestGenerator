// Copyright 2022 Jan Plähn. All Rights Reserved.

#include "CharacterHasItemCondition.h"

#include "AesirProceduralQuestBPLibrary.h"
#include "QuestGenerator/WorldState/SimulatedWorldStateComponent.h"

bool UCharacterHasItemCondition::IsResolved_Implementation(const UObject* WorldContextObject) const
{
	USimulatedWorldStateComponent* WorldStateComponent = USimulatedWorldStateComponent::TryFindWorldStateComponent(WorldContextObject);
	if (!IsValid(WorldStateComponent))
	{
		return bInvertCondition;
	}
	FCharacterInventory* CharacterInventory = WorldStateComponent->CharacterHasItemMap.Find(CharacterName);
	if (CharacterInventory == nullptr)
	{
		return bInvertCondition;
	}
	return CharacterInventory->Items.Contains(ItemName) != bInvertCondition;
}

FString UCharacterHasItemCondition::GetPropertyInfo_Implementation() const
{
	FString BaseString = Super::GetPropertyInfo_Implementation();
	BaseString.Append(FString::Printf(TEXT("Item: %s; "), *ItemName.ToString()));
	BaseString.Append(FString::Printf(TEXT("Character: %s; "), *CharacterName.ToString()));	
	return BaseString;
}

uint32 UCharacterHasItemCondition::GenerateId() const
{
	const uint32 TypeHash = GetTypeHash(GetClass());
	const uint32 ItemNameHash = TextKeyUtil::HashString(ItemName.ToString());
	const uint32 CharacterNameHash = TextKeyUtil::HashString(CharacterName.ToString());
	const uint32 CombinedParameterHash = HashCombine(CharacterNameHash, ItemNameHash);
	const uint32 CombinedHash = HashCombine(TypeHash, CombinedParameterHash);
	return CombinedHash;
}

void UCharacterHasItemCondition::InjectParameters(const TArray<UQuestParameter*>& Parameters)
{
	UAesirProceduralQuestBPLibrary::InjectNameParameter(ItemName, Parameters);
	UAesirProceduralQuestBPLibrary::InjectNameParameter(CharacterName, Parameters);
	Super::InjectParameters(Parameters);
}

TSet<FName> UCharacterHasItemCondition::GetParameters() const
{
	return TSet<FName>
	{
		CharacterName,
		ItemName
	};
}
