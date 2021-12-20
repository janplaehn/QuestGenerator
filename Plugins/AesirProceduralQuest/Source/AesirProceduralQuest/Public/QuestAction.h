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
	virtual bool IsAvailable(const UObject* WorldContextObject) const;

	virtual bool SimulateIsAvailable(const UObject* WorldContextObject, TMap<uint32, bool> SimulatedConditionResolutions) const;

	virtual bool IsResolved(const UObject* WorldContextObject) const;

	const TArray<UQuestCondition*>& GetPreConditions() const;

	const TArray<UQuestCondition*>& GetPostConditions() const;

protected:
	UPROPERTY(EditDefaultsOnly, Instanced)
	TArray<UQuestCondition*> PreConditions;

	UPROPERTY(EditDefaultsOnly, Instanced)
	TArray<UQuestCondition*> PostConditions;

	UPROPERTY(EditDefaultsOnly)
	FQuestLabelCollection Labels;	
};
 