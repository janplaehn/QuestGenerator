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
	
	static UQuest* SelectFittest(UQuest* QuestA, UQuest* QuestB, UQuestProviderPreferences* Preferences);

protected:	
	static float CalculateFitness(UQuest* Quest, UQuestProviderPreferences* Preferences);
	
	static float CalculateFitnessByDesiredConditions(UQuest* Quest, UQuestProviderPreferences* Preferences);
	
};
