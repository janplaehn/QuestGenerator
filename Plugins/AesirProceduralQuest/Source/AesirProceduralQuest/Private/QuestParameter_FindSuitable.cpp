// Fill out your copyright notice in the Description page of Project Settings.

#include "QuestParameter_FindSuitable.h"

FName UQuestParameter_FindSuitable::GenerateValue(const UQuest* Quest) const
{	
	//Try generate value from previous action
	const TArray<TWeakObjectPtr<UQuestAction>>& Actions = Quest->GetActions();
	if (Actions.Num() && FMath::RandBool())
	{
		const TWeakObjectPtr<UQuestAction>& PreviousAction = Actions.Last();
		if (const TSet<FName>* NameSet = PreviousAction->ParametersByClass.Find(StaticClass()))
		{
			const int RandomIndex = FMath::RandRange(0, NameSet->Num()-1);
			return NameSet->Array()[RandomIndex]; //Todo: Find a faster way to get a random element, this loops through the whole set!!
		}
	}

	//Generate value from world State
	if (FMath::RandBool())
	{
		FName OutValue;
		if (GenerateValueFromWorldState(Quest, OutValue))
		{
			return OutValue;
		}
	}
	
	return Super::GenerateValue(Quest);
}
