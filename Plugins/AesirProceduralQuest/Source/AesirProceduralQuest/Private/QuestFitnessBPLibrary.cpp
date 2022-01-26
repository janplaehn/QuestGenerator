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
	const float ConditionFitnessA = CalculateFitnessByDesiredConditions(WorldContextObject, QuestA, Preferences);
	const float ConditionFitnessB = CalculateFitnessByDesiredConditions(WorldContextObject, QuestB, Preferences);

	if (!FMath::IsNearlyEqual(ConditionFitnessA, ConditionFitnessB, 0.1f))
	{
		return ConditionFitnessA > ConditionFitnessB ? QuestA : QuestB;
	}
	
	const float FitnessA = CalculateWeightedFitness(WorldContextObject, QuestA, Preferences);
	const float FitnessB = CalculateWeightedFitness(WorldContextObject, QuestB, Preferences);
	return FitnessA > FitnessB ? QuestA : QuestB;
}

float UQuestFitnessUtils::CalculateWeightedFitness(const UObject* WorldContextObject, const UQuest* Quest, const UQuestProviderPreferences* Preferences)
{
	//Todo: Give Fitness values weights based on the preferences!
	const float FitnessByTags = CalculateFitnessByTags(Quest, Preferences) * Preferences->FitnessWeights.TagWeight;
	const float FitnessByConditions = CalculateFitnessByDesiredConditions(WorldContextObject, Quest, Preferences) * Preferences->FitnessWeights.ConditionWeight;
	const float FitnessByIntentionality = CalculateFitnessByIntentionality(Quest) * Preferences->FitnessWeights.IntentionalityWeight;
	return FitnessByTags + FitnessByConditions + FitnessByIntentionality;
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

float UQuestFitnessUtils::CalculateFitnessByIntentionality(const UQuest* Quest)
{
	if (!IsValid(Quest))
	{
		return 0.0f;
	}
	
	const TArray<const UQuestAction*>& Actions = Quest->GetActions();
	const int ConnectionCount = Actions.Num() - 1;
	float AverageConditionMatchup = 0;
	for (int ActionIndex = 0; ActionIndex < ConnectionCount; ActionIndex++)
	{
		const UQuestAction* PreviousAction = Actions[ActionIndex];
		const UQuestAction* NextAction = Actions[ActionIndex+1];
		AverageConditionMatchup += UAesirProceduralQuestBPLibrary::GetActionListSimilarity(PreviousAction->GetPostConditions(), NextAction->GetPreConditions());
	}
	return AverageConditionMatchup / ConnectionCount;
}