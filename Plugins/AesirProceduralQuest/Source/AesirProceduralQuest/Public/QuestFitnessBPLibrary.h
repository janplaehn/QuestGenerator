// Copyright 2022 Jan Plähn. All Rights Reserved.

#pragma once

class UQuestProviderPreferences;
class UQuest;

/* 
*	
*/
class AESIRPROCEDURALQUEST_API UQuestFitnessUtils
{
public:
	UQuestFitnessUtils();
	
	static const UQuest* SelectFittest(const UObject* WorldContextObject, const UQuest* QuestA, const UQuest* QuestB, const UQuestProviderPreferences* Preferences);

	static float CalculateFitness(const UObject* WorldContextObject, const UQuest* Quest, const UQuestProviderPreferences* Preferences);
    
    static float CalculateFitnessByDesiredConditions(const UObject* WorldContextObject, const UQuest* Quest, const UQuestProviderPreferences* Preferences);
   
    static float CalculateFitnessByTags(const UQuest* Quest, const UQuestProviderPreferences* Preferences);
	
};
