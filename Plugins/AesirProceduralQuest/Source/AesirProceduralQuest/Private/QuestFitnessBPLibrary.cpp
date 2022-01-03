// Copyright 2022 Jan Plähn. All Rights Reserved.

#include "QuestFitnessBPLibrary.h"
#include "Quest.h"
#include "QuestProviderPreferences.h"

UQuestFitnessUtils::UQuestFitnessUtils()
{
}

UQuest* UQuestFitnessUtils::SelectFittest(UQuest* QuestA, UQuest* QuestB, UQuestProviderPreferences* Preferences)
{
	const float FitnessA = CalculateFitness(QuestA, Preferences);
	const float FitnessB = CalculateFitness(QuestB, Preferences);
	return FitnessA > FitnessB ? QuestA : QuestB;
}

float UQuestFitnessUtils::CalculateFitness(UQuest* Quest, UQuestProviderPreferences* Preferences)
{
	//Todo: Calculate multiple fitness-values and give them weights based on the preferences!
	return CalculateFitnessByDesiredConditions(Quest, Preferences);
}

float UQuestFitnessUtils::CalculateFitnessByDesiredConditions(UQuest* Quest, UQuestProviderPreferences* Preferences)
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
	for (const UQuestCondition* DesiredCondition : Preferences->DesiredConditions)
	{
		for (const UQuestCondition* QuestPostCondition : QuestPostConditions)
		{
			if (DesiredCondition->GetClass() == QuestPostCondition->GetClass())
			{
				UE_LOG(LogTemp, Warning, TEXT("SAME CLASS UH OH"));
			}
			
			if (DesiredCondition->GetId() != QuestPostCondition->GetId())
			{
				continue;
			}

			if (DesiredCondition->bInvertCondition != QuestPostCondition->bInvertCondition)
			{
				Fitness -= 1.0f/ConditionCount;
			}
		}
	}
	
	return Fitness;
}
