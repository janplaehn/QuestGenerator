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

bool UQuest::CopyFrom(const UQuest* OtherQuest)
{
	if (OtherQuest == this)
	{
		return false;
	}
	Clear();
	for (auto Action : OtherQuest->Actions)
	{
		//Todo: Don't duplicate these objects. Store them in a database instead and unload them if no longer needed!
		Actions.Add(DuplicateObject(Action, this));
	}
	return true;
}

void UQuest::AddQuestAction(UQuestAction* NewAction)
{
	Actions.Add(NewAction);
}

void UQuest::Clear()
{
	for (auto Action : Actions)
	{
		Action->ConditionalBeginDestroy();
	}
	Actions.Empty();
	
	//Todo: Copy Fitness from other quest? (Better: have this stored in the Snapshot anyways)
	CachedFitnessByAffinity = -1;
	CachedFitnessByConditions = -1;
	CachedFitnessByIntentionality = -1;
}

bool UQuest::IsEmpty() const
{
	return !Actions.Num();
}

const TArray<UQuestAction*>& UQuest::GetActions() const
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
	if (CachedFitnessByConditions < 0)
	{
		CachedFitnessByConditions = UQuestFitnessUtils::CalculateFitnessByDesiredConditions(Context, this, ProviderData);
	}
	return CachedFitnessByConditions;
}

float UQuest::GetFitnessByIntentionality()
{
	if (CachedFitnessByIntentionality < 0)
	{
		CachedFitnessByIntentionality = UQuestFitnessUtils::CalculateFitnessByIntentionality(this);
	}
	//Todo: Cache fitness in snapshots instead!
	return CachedFitnessByIntentionality;
}

float UQuest::GetFitnessByAffinity()
{
	if (CachedFitnessByAffinity < 0)
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
