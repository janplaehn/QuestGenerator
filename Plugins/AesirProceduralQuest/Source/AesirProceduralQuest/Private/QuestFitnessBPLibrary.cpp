// Copyright 2022 Jan Plähn. All Rights Reserved.

#include "QuestFitnessBPLibrary.h"
#include "AesirProceduralQuestBPLibrary.h"
#include "Quest.h"
#include "QuestProviderPreferences.h"

UQuestFitnessUtils::UQuestFitnessUtils()
{
}

UQuest* UQuestFitnessUtils::SelectSimulatedAnnealingFittest(const UObject* WorldContextObject, UQuest* QuestA,
	UQuest* QuestB, const UQuestProviderPreferences* Preferences)
{
	UQuest* Fittest = SelectFittest(WorldContextObject, QuestA, QuestB, Preferences);
	if (Fittest == QuestB)
	{
		return QuestB;
	}
	const float Random = FMath::Pow(FMath::FRand(), 32);
	if (Random > CalculateWeightedFitness(WorldContextObject, QuestA, Preferences))
	{
		return QuestB;
	}
	return QuestA;
}

UQuest* UQuestFitnessUtils::SelectFittest(const UObject* WorldContextObject, UQuest* QuestA, UQuest* QuestB, const UQuestProviderPreferences* Preferences)
{
	const float FitnessA = CalculateWeightedFitness(WorldContextObject, QuestA, Preferences);
	const float FitnessB = CalculateWeightedFitness(WorldContextObject, QuestB, Preferences);
	return FitnessA >= FitnessB ? QuestA : QuestB;
}

float UQuestFitnessUtils::CalculateWeightedFitness(const UObject* WorldContextObject, UQuest* Quest, const UQuestProviderPreferences* Preferences)
{
	if (!IsValid(Quest) || !Quest->GetActions().Num())
	{
		return 0.0f;
	}

	const float FitnessByConditions = Quest->GetFitnessByConditions(WorldContextObject) * Preferences->FitnessWeights.ConditionWeight;
	const float FitnessByIntentionality = Quest->GetFitnessByIntentionality() * Preferences->FitnessWeights.IntentionalityWeight;
	const float FitnessByAffinity = Quest->GetFitnessByAffinity() * Preferences->FitnessWeights.AffinityWeight;
	return FitnessByConditions + FitnessByIntentionality + FitnessByAffinity;
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

float UQuestFitnessUtils::CalculateFitnessByAffinity(UQuest* Quest, const UQuestProviderPreferences* Preferences)
{
	if (!IsValid(Quest))
	{
		return 0.0f;
	}
	float AverageAffinityMatch = 0;
	const TArray<UQuestAction*>& Actions = Quest->GetActions();
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
	const TArray<UQuestAction*>& Actions = Quest->GetActions();
	if (!Actions.Num())
	{
		return 0.0f;
	}
	
	float AverageConditionMatchup = 0;
	for (int ActionIndex = 0; ActionIndex < Actions.Num(); ActionIndex++)
	{
		const TArray<UQuestCondition*>& PreviousConditions = Actions[ActionIndex]->GetPostConditions();
		const TArray<UQuestCondition*>& NextConditions =  ActionIndex < Actions.Num() - 1 ? Actions[ActionIndex+1]->GetPreConditions() : Quest->GetProviderData()->DesiredConditions.Array();
		const float Similarity = UAesirProceduralQuestBPLibrary::GetActionListSimilarity(PreviousConditions, NextConditions);
		AverageConditionMatchup += FMath::IsNearlyZero(Similarity) ? 0.0f : 1.0f;
	}
	
	return AverageConditionMatchup / Actions.Num();
}
