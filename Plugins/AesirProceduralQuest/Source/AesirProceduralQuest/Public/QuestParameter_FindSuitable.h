// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestParameter.h"
#include "QuestParameter_RandomFromTable.h"
#include "QuestParameter_FindSuitable.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Abstract)
class AESIRPROCEDURALQUEST_API UQuestParameter_FindSuitable : public UQuestParameter_RandomFromTable
{
	GENERATED_BODY()
public:
	virtual FName GenerateValue(const UQuest* Quest) const override;

protected:
	virtual bool GenerateValueFromWorldState(const UQuest* Quest, FName& OutValue) const PURE_VIRTUAL(UQuestParameter::GetValueAsName, return false;);
};
