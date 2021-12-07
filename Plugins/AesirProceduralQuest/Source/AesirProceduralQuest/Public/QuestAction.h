// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestCondition.h"
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
	UPROPERTY(EditDefaultsOnly)
	TArray<UQuestCondition*> PreConditions;

	UPROPERTY(EditDefaultsOnly)
	TArray<UQuestCondition*> PostConditions;

	UPROPERTY(EditDefaultsOnly)
	TArray<FName> Labels;	
};
 