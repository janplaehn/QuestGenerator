// Copyright 2022 Jan Plähn. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "QuestCondition.h"
#include "QuestLabel.h"
#include "QuestAction.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class AESIRPROCEDURALQUEST_API UQuestAction : public UDataAsset
{
	GENERATED_BODY()

public:
	virtual bool IsAvailable() const;

	virtual bool IsResolved() const;

protected:
	UPROPERTY(EditDefaultsOnly, Instanced)
	TArray<UQuestCondition*> PreConditions;

	UPROPERTY(EditDefaultsOnly, Instanced)
	TArray<UQuestCondition*> PostConditions;

	UPROPERTY(EditDefaultsOnly)
	TArray<FQuestLabel> Labels;	
};
 