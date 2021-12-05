#pragma once

#include "CoreMinimal.h"
#include "QuestData.generated.h"

class UQuest;

USTRUCT(BlueprintType)
struct FQuestData
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
