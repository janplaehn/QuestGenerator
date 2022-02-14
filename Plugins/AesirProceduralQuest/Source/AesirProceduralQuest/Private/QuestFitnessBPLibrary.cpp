// Copyright 2022 Jan Plähn. All Rights Reserved.

#include "QuestFitnessBPLibrary.h"
#include "AesirProceduralQuestBPLibrary.h"
#include "Quest.h"
#include "QuestProviderPreferences.h"

UQuestFitnessUtils::UQuestFitnessUtils()
{
}

UQuest* UQuestFitnessUtils::SelectFittest(const UObject* WorldContextObject, UQuest* QuestA, UQuest* QuestB, const UQuestProviderPreferences* Preferences)
{
	// const float ConditionFitnessA = !IsValid(QuestA) ? 0.0f : QuestA->GetFitnessByConditions(WorldContextObject);
	// const float ConditionFitnessB = !IsValid(QuestB) ? 0.0f : QuestB->GetFitnessByConditions(WorldContextObject);
	//
	// if (!FMath::IsNearlyEqual(ConditionFitnessA, ConditionFitnessB, 0.1f))
	// {
	// 	return ConditionFitnessA > ConditionFitnessB ? QuestA : QuestB;
	// }
	
	const float FitnessA = CalculateWeightedFitness(WorldContextObject, QuestA, Preferences);
	const float FitnessB = CalculateWeightedFitness(WorldContextObject, QuestB, Preferences);
	return FitnessA >= FitnessB ? QuestA : QuestB;
}

float UQuestFitnessUtils::CalculateWeightedFitness(const UObject* WorldContextObject, UQuest* Quest, const UQuestProviderPreferences* Preferences)
{
	if (!IsValid(Quest))
	{
		return 0.0f;
	}
	const float FitnessByTags = Quest->GetFitnessByTags() * Preferences->FitnessWeights.TagWeight;
	const float FitnessByConditions = Quest->GetFitnessByConditions(WorldContextObject) * Preferences->FitnessWeights.ConditionWeight;
	const float FitnessByIntentionality = Quest->GetFitnessByIntentionality() * Preferences->FitnessWeights.IntentionalityWeight;
	const float FitnessByAffinity = Quest->GetFitnessByAffinity() * Preferences->FitnessWeights.AffinityWeight;
	const float FitnessByDuplicates = Quest->GetFitnessByDuplicates() * Preferences->FitnessWeights.DuplicateWeight;
	return FitnessByTags + FitnessByConditions + FitnessByIntentionality + FitnessByAffinity + FitnessByDuplicates;
}

float UQuestFitnessUtils::CalculateFitnessByDesiredConditions(const UObject* WorldContextObject, UQuest* Quest, const UQuestProviderPreferences* Preferences)
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

float UQuestFitnessUtils::CalculateFitnessByTags(UQuest* Quest, const UQuestProviderPreferences* Preferences)
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

float UQuestFitnessUtils::CalculateFitnessByAffinity(UQuest* Quest, const UQuestProviderPreferences* Preferences)
{
	if (!IsValid(Quest))
	{
		return 0.0f;
	}
	float AverageAffinityMatch = 0;
	const TArray<const UQuestAction*>& Actions = Quest->GetActions();
	for (const UQuestAction* Action : Actions)
	{
		float NewAffinityMatch;
		if (Action->CharacterImpact.Character.IsNone())
		{
			NewAffinityMatch = 0.5f;
		}
		else if (Action->CharacterImpact.Character == Preferences->ProviderName)
		{
			NewAffinityMatch = 1 - FMath::Abs(1 - Action->CharacterImpact.Affinity) / 2.0f;
		}
		else
		{
			const auto FoundAffinity =  Preferences->CharacterAffinities.FindByPredicate([&Action](const FCharacterAffinity& Affinity)
			{
				return Action->CharacterImpact.Character== Affinity.Character;
			});
			if (FoundAffinity == nullptr)
			{
				NewAffinityMatch = 1 - FMath::Abs(Preferences->DefaultAffinity - Action->CharacterImpact.Affinity) / 2.0f;
			}
			else
			{
				NewAffinityMatch = 1 - FMath::Abs((*FoundAffinity).Affinity - Action->CharacterImpact.Affinity) / 2.0f;
			}
		}
		AverageAffinityMatch += NewAffinityMatch;
	}
	return AverageAffinityMatch / Quest->GetActions().Num();
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

float UQuestFitnessUtils::CalculateFitnessByDuplicates(UQuest* Quest)
{
	if (!IsValid(Quest))
	{
		return 0.0f;
	}
	const TArray<const UQuestAction*>& Actions = Quest->GetActions();
	TSet<uint32> Ids;
	for (const UQuestAction* Action : Actions)
	{
		uint32 Id = Action->GetId();
		if (Ids.Contains(Id))
		{
			return 0.0f;
		}
		Ids.Add(Id);
	}
	return 1.0f;
}
