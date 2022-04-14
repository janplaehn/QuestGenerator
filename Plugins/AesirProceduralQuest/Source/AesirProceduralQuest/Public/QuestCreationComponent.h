﻿// Copyright 2022 Jan Plähn. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "QuestProviderComponent.h"
#include "QuestProviderPreferences.h"
#include "Components/ActorComponent.h"
#include "QuestCreationComponent.generated.h"

class UQuest;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestUpdated, UQuest*, Quest);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable, BlueprintType)
class AESIRPROCEDURALQUEST_API UQuestCreationComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UQuestCreationComponent();

	UFUNCTION(BlueprintCallable)
	void Initialize();
	
	void RequestQuestGeneration(UQuestProviderComponent* QuestProviderComponent);
	
	void PauseQuestGeneration(UQuestProviderComponent* QuestProviderComponent);

	UPROPERTY(BlueprintAssignable)
	FOnQuestUpdated OnQuestUpdated;

	UPROPERTY(BlueprintAssignable)
	FOnQuestUpdated OnQuestCompleted;

protected:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	/**
	* DataTable that holds all possible quest actions
	* */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = ( RowType="QuestActionRow" ))
	UDataTable* QuestActionDataTable;

	UQuest* CreateRandomQuest(const uint32 QuestActionCount, const UQuestProviderComponent * ProviderComponent);

	UQuest* MutateQuest(UQuest* BaseQuest, const int32 QuestActionCount, const UQuestProviderComponent * ProviderComponent);

	UQuest* MutateQuestByReplaceAction(UQuest* BaseQuest, const int32 QuestActionCount, const UQuestProviderComponent* ProviderComponent);

	UQuest* MutateQuestByScramblingActions(UQuest* BaseQuest, const int32 QuestActionCount, const UQuestProviderComponent* ProviderComponent);

	UPROPERTY(EditAnywhere)
	FInt32Range QuestActionCountRange = FInt32Range(5,10);

	UPROPERTY(EditAnywhere)
	int GenerationIterationsPerFrame = 5.0f;

	UPROPERTY(EditAnywhere)
	int MaxQuestSampleCount = 30;

	UPROPERTY(EditAnywhere)
	bool bEnableConditionMatching = true;

	UPROPERTY(EditAnywhere)
	int32 IterationsToAbandonLocalMaximum = 500;

	bool TryApplyRandomNextQuestAction(UQuest* Quest, TMap<uint32, uint32>& SimulatedConditionResolutions) const;

	bool TryApplyNextQuestAction(UQuest* Quest, UQuestAction* ActionCandidate, TMap<uint32, uint32>& SimulatedConditionResolutions) const;

	UQuestAction* GetRandomQuestAction(UObject* Outer = nullptr) const;

	void InitPossibleQuestActions();

	UPROPERTY(Transient)
	TArray<UQuestAction*> CachedPossibleQuestActions;

	UPROPERTY(Transient)
	TSet<UQuestProviderComponent*> QuestRequesters;

private:
	TWeakObjectPtr<UQuest> LocalMaximumQuest;
	double StartTimestamp;
	double LastLogTimestamp;
	int32 IterationsSinceLastGlobalImprovement = 0;
	int32 IterationsSinceLastLocalImprovement = 0;
};
