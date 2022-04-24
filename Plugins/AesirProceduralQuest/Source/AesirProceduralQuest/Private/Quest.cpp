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
	for (const TWeakObjectPtr<UQuestAction>& Action : Actions)
	{
		if (!Action->IsResolved(WorldContextObject))
		{
			return false;
		}
	}
	return true;
}

void UQuest::ClearQuest()
{
	Actions.RemoveAll([](TWeakObjectPtr<UQuestAction>& Action)
	{
		Action->OwnerCount--;
		return true;
	});
	
	//Todo: Copy Fitness from other quest? (Better: have this stored in the Snapshot anyways)
	CachedFitnessByAffinity = -1;
	CachedFitnessByConditions = -1;
	CachedFitnessByIntentionality = -1;
}

bool UQuest::CopyFrom(const UQuest* OtherQuest)
{
	if (OtherQuest == this)
	{
		return false;
	}
	ClearQuest();
	Actions.Reserve(OtherQuest->Actions.Num());
	for (TWeakObjectPtr<UQuestAction> Action : OtherQuest->GetActions())
	{
		AddQuestAction(Action);
	}
	return true;
}

void UQuest::AddQuestAction(TWeakObjectPtr<UQuestAction> NewAction)
{
	NewAction->OwnerCount++;
	Actions.Add(NewAction);
}

bool UQuest::IsEmpty() const
{
	return !Actions.Num();
}

TArray<UQuestAction*> UQuest::BlueprintGetActions() const
{
	TArray<UQuestAction*> OutActions;
	OutActions.Reserve(Actions.Num());
	for (auto Action : Actions)
	{
		OutActions.Add(Action.Get());
	}
	return OutActions;
}

TArray<TWeakObjectPtr<UQuestAction>> UQuest::GetActions() const
{
	return Actions;
}

TArray<UQuestCondition*> UQuest::GetPostConditions() const
{
	TArray<UQuestCondition*> OutPostConditions;
	for (const TWeakObjectPtr<UQuestAction> Action : Actions)
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
