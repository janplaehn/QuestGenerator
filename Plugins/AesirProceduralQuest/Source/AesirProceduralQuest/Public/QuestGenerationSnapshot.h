// Copyright 2022 Jan Plähn. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "QuestGenerationSnapshot.generated.h"

class UQuestProviderPreferences;
class UQuest;

USTRUCT(BlueprintType)
struct AESIRPROCEDURALQUEST_API FQuestGenerationSnapshot
{
	GENERATED_BODY()
public:
	FQuestGenerationSnapshot() = default;
	
	explicit FQuestGenerationSnapshot(UObject* InContext, UQuestProviderPreferences* InGenerationData);
	
	UPROPERTY(Transient, BlueprintReadOnly)
	UQuest* GlobalMaximum;

	UPROPERTY(Transient, BlueprintReadOnly)
	UQuest* LocalMaximum;

	//Todo: Actually just store this candidate in the QuestCreationComponent. We don't need this in the snapshot (or do we?)
	UPROPERTY(Transient, BlueprintReadOnly)
	UQuest* Candidate;
	
	TWeakObjectPtr<UObject> Context;

	UPROPERTY(Transient, BlueprintReadOnly)
	TWeakObjectPtr<UQuestProviderPreferences> GenerationData;

	UPROPERTY(Transient, BlueprintReadOnly)
	int32 TotalIterations = 0;

	UPROPERTY(Transient, BlueprintReadOnly)
	int32 NullQuestCount = 0;

	UPROPERTY(Transient, BlueprintReadOnly)
	int32 IterationsSinceLastGlobalImprovement = 0;

	UPROPERTY(Transient, BlueprintReadOnly)
	int32 IterationsSinceLastLocalImprovement = 0;

	UPROPERTY(Transient, BlueprintReadOnly)
	float GlobalMaximumFitness = -1;

	UPROPERTY(Transient, BlueprintReadOnly)
	float LocalMaximumFitness = -1;
};