// Copyright 2022 Jan Plähn. All Rights Reserved.

#pragma once

#include "QuestFitnessBPLibrary.generated.h"

class UQuestProviderPreferences;
class UQuest;

/* 
*	
*/
UCLASS()
class AESIRPROCEDURALQUEST_API UQuestFitnessUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UQuestFitnessUtils();
	
	static UQuest* SelectSimulatedAnnealingFittest(const UObject* WorldContextObject, UQuest* QuestA, UQuest* QuestB, const UQuestProviderPreferences* Preferences);
	
	static UQuest* SelectFittest(const UObject* WorldContextObject, UQuest* QuestA, UQuest* QuestB, const UQuestProviderPreferences* Preferences);

	UFUNCTION(BlueprintPure)
	static float CalculateWeightedFitness(const UObject* WorldContextObject, UQuest* Quest, const UQuestProviderPreferences* Preferences);
    
    static float CalculateFitnessByDesiredConditions(const UObject* WorldContextObject, UQuest* Quest, const UQuestProviderPreferences* Preferences);

	static float CalculateFitnessByAffinity(UQuest* Quest, const UQuestProviderPreferences* Preferences);

	static float CalculateFitnessByIntentionality(const UQuest* Quest);
};
