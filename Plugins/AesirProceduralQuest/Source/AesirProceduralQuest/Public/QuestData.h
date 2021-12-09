// Copyright 2022 Jan Plähn. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "QuestData.generated.h"

class UQuest;

USTRUCT(BlueprintType)
struct AESIRPROCEDURALQUEST_API FQuestData
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadOnly)
	UQuest* Quest;
	
	UPROPERTY(BlueprintReadOnly)
	FText Description;

	UPROPERTY(BlueprintReadOnly)
	FText DialogueText;
};
