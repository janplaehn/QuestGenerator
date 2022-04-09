// Copyright 2022 Jan Plähn. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "QuestCondition.h"
#include "QuestLabel.h"
#include "Engine/DataAsset.h"
#include "QuestProviderPreferences.generated.h"

class UQuestAction;

/**
* Parameters for Fitness Weights
*/
USTRUCT(BlueprintType) 
struct AESIRPROCEDURALQUEST_API FQuestFitnessWeights
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Min = 0.0f, Max = 1.0f))
	float ConditionWeight = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Min = 0.0f, Max = 1.0f))
	float IntentionalityWeight = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Min = 0.0f, Max = 1.0f))
	float AffinityWeight = 0.5f;
};

/**
* Parameters for CharacterAffinities
*/
USTRUCT(BlueprintType) 
struct AESIRPROCEDURALQUEST_API FCharacterAffinity
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName Character;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Min = -1, Max = 1, ClampMin = -1, ClampMax = 1))
	int Affinity = 0;
};

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class AESIRPROCEDURALQUEST_API UQuestProviderPreferences : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName ProviderName = "GenericQuestProvider";

	UPROPERTY(EditAnywhere)
	FQuestFitnessWeights FitnessWeights = FQuestFitnessWeights();

	UPROPERTY(EditAnywhere, Instanced)
	TSet<UQuestCondition*> DesiredConditions;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Min = -1, Max = 1, ClampMin = -1, ClampMax = 1))
	int DefaultAffinity = 0;
	
	UPROPERTY(EditAnywhere)
	TArray<FCharacterAffinity> CharacterAffinities;
};
