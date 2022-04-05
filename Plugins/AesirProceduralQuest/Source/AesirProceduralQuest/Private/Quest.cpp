// Copyright 2022 Jan Plähn. All Rights Reserved.

#include "Quest.h"

#include "QuestFitnessBPLibrary.h"

bool UQuest::IsAvailable(const UObject* WorldContextObject) const
{
	if (Actions.Num() == 0)
	{
		return true;
	}
	return Actions[0]->IsAvailable(WorldContextObject);
}

bool UQuest::IsResolved(const UObject* WorldContextObject) const
{
	for (const TSoftObjectPtr<UQuestAction> Action : Actions)
	{
		if (!Action->IsResolved(WorldContextObject))
		{
			return false;
		}
	}
	return true;
}

void UQuest::AddQuestAction(const UQuestAction* NewAction)
{
	Actions.Add(NewAction);
}

const TArray<const UQuestAction*>& UQuest::GetActions() const
{
	return Actions;
}

TArray<UQuestCondition*> UQuest::GetPostConditions() const
{
	TArray<UQuestCondition*> OutPostConditions;
	for (const UQuestAction* Action : Actions)
	{
		OutPostConditions.Append(Action->GetPostConditions());
	}
	return OutPostConditions;
}

float UQuest::GetFitnessByConditions(const UObject* Context)
{
	if (CachedFitnessByConditions == -1)
	{
		CachedFitnessByConditions = UQuestFitnessUtils::CalculateFitnessByDesiredConditions(Context, this, ProviderData);
	}
	return CachedFitnessByConditions;
}

float UQuest::GetFitnessByIntentionality()
{
	if (CachedFitnessByIntentionality == -1)
	{
		CachedFitnessByIntentionality = UQuestFitnessUtils::CalculateFitnessByIntentionality(this);
	}
	return CachedFitnessByIntentionality;
}

float UQuest::GetFitnessByAffinity()
{
	if (CachedFitnessByAffinity == -1)
	{
		CachedFitnessByAffinity = UQuestFitnessUtils::CalculateFitnessByAffinity(this, ProviderData);
	}
	return CachedFitnessByAffinity;	
}

void UQuest::SetProviderData(UQuestProviderPreferences* Data)
{
	ProviderData = Data;
}

UQuestProviderPreferences* UQuest::GetProviderData() const
{
	return ProviderData;
}

float UQuest::GetFitnessByDuplicates()
{
	if (CachedFitnessByDuplicates == -1)
	{
		CachedFitnessByDuplicates = UQuestFitnessUtils::CalculateFitnessByDuplicates(this);
	}
	return CachedFitnessByDuplicates;
}
