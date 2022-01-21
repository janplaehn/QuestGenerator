// Copyright 2022 Jan Plähn. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "QuestCondition.h"
#include "QuestLabel.h"
#include "Engine/DataAsset.h"
#include "QuestProviderPreferences.generated.h"

class UQuestAction;

/**
* Parameters for Crashing
*/
USTRUCT(BlueprintType) 
struct AESIRPROCEDURALQUEST_API FQuestFitnessWeights
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Min = 0.0f, Max = 1.0f))
	float ConditionWeight = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Min = 0.0f, Max = 1.0f))
	float IntentionalityWeight = 0.35f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Min = 0.0f, Max = 1.0f))
	float TagWeight = 0.15f;
};

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class AESIRPROCEDURALQUEST_API UQuestProviderPreferences : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	FName ProviderName = "GenericQuestProvider";

	UPROPERTY(EditAnywhere)
	FQuestFitnessWeights FitnessWeights = FQuestFitnessWeights();

	UPROPERTY(EditAnywhere)
	FQuestLabelCollection AssociatedLabels;

	UPROPERTY(EditAnywhere, Instanced)
	TSet<UQuestCondition*> DesiredConditions;
};
