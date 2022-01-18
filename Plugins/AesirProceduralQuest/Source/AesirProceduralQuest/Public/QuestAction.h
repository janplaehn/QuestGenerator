// Copyright 2022 Jan Plähn. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "QuestCondition.h"
#include "QuestLabel.h"
#include "QuestParameter.h"

#include "QuestAction.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class AESIRPROCEDURALQUEST_API UQuestAction : public UDataAsset
{
	GENERATED_BODY()

public:
	TArray<UQuestAction*> MakeAllInstances() const;

	void MakeCombination(TArray<uint32>& InOutCurrentCombination, TArray<uint32> IndexCounts);
	
	UQuestAction* MakeRandomInstance() const;
	
	virtual void InitializeAsInstance();

	uint32 GetPossibleInstanceCount() const;
	
	virtual bool IsAvailable(const UObject* WorldContextObject) const;

	virtual bool SimulateIsAvailable(const UObject* WorldContextObject, TMap<uint32, bool> SimulatedConditionResolutions) const;

	virtual bool IsResolved(const UObject* WorldContextObject) const;

	const TArray<UQuestCondition*>& GetPreConditions() const;

	virtual TArray<UQuestCondition*> GetPostConditions() const;

	FText GetFormattedHumanReadableName() const;
	
	UPROPERTY(EditAnywhere)
	FQuestLabelCollection AssociatedLabels;
	
protected:
	UPROPERTY(EditDefaultsOnly, Instanced)
	TArray<UQuestCondition*> PreConditions;

	UPROPERTY(EditDefaultsOnly, Instanced)
	TArray<UQuestCondition*> PostConditions;

private:
	UPROPERTY(EditDefaultsOnly)
	FText ReadableDescription;
	
	UPROPERTY(EditAnywhere, Instanced)
	TArray<UQuestParameter*> Parameters;
};
 