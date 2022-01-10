// Copyright 2022 Jan Plähn. All Rights Reserved.

#include "QuestFitnessBPLibrary.h"

#include "AesirProceduralQuestBPLibrary.h"
#include "Quest.h"
#include "QuestProviderPreferences.h"

UQuestFitnessUtils::UQuestFitnessUtils()
{
}

const UQuest* UQuestFitnessUtils::SelectFittest(const UObject* WorldContextObject, const UQuest* QuestA, const UQuest* QuestB, const UQuestProviderPreferences* Preferences)
{
	const float FitnessA = CalculateFitness(WorldContextObject, QuestA, Preferences);
	const float FitnessB = CalculateFitness(WorldContextObject, QuestB, Preferences);
	return FitnessA > FitnessB ? QuestA : QuestB;
}

float UQuestFitnessUtils::CalculateFitness(const UObject* WorldContextObject, const UQuest* Quest, const UQuestProviderPreferences* Preferences)
{
	//Todo: Give Fitness values weights based on the preferences!
	const float FitnessByTags = CalculateFitnessByTags(Quest, Preferences) * 0.3f;
	const float FitnessByConditions = CalculateFitnessByDesiredConditions(WorldContextObject, Quest, Preferences) * 0.7f;
	return FitnessByTags + FitnessByConditions;
}

float UQuestFitnessUtils::CalculateFitnessByDesiredConditions(const UObject* WorldContextObject, const UQuest* Quest, const UQuestProviderPreferences* Preferences)
{
	if (!IsValid(Quest))
	{
		return 0.0f;
	}

	const int ConditionCount = Preferences->DesiredConditions.Num();
	if (ConditionCount == 0)
	{
		return 1.0f;
	}

	float Fitness = 1.0f;
	const TArray<UQuestCondition*>& QuestPostConditions = Quest->GetPostConditions();
	for (const UQuestCondition* DesiredCondition : Preferences->DesiredConditions) //Todo: Use FindByPredicate instead?
	{
		bool bFoundMatchingCondition = false;
		for (const UQuestCondition* QuestPostCondition : QuestPostConditions)
		{
			
			if (DesiredCondition->GetId() != QuestPostCondition->GetId())
			{
				continue;
			}
			bFoundMatchingCondition = true;
			if (DesiredCondition->bInvertCondition != QuestPostCondition->bInvertCondition)
			{
				Fitness -= 1.0f/ConditionCount;
			}
			break;
		}
		if (!bFoundMatchingCondition && !DesiredCondition->IsResolved(WorldContextObject))
		{
			Fitness -= 1.0f/ConditionCount;
		}
	}
	
	return Fitness;
}

float UQuestFitnessUtils::CalculateFitnessByTags(const UQuest* Quest, const UQuestProviderPreferences* Preferences)
{
	if (!IsValid(Quest))
    {
    	return 0.0f;
    }
	float AverageSimilarity = 0;
	const TArray<const UQuestAction*>& Actions = Quest->GetActions();
	for (const UQuestAction* Action : Actions)
	{
		AverageSimilarity += UAesirProceduralQuestBPLibrary::GetListSimilarity(Action->AssociatedLabels.Labels, Preferences->AssociatedLabels.Labels);
	}
	return AverageSimilarity / Quest->GetActions().Num();
}
