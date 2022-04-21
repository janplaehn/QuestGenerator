// Copyright 2022 Jan Plähn. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "QuestGenerationSnapshot.h"
#include "QuestProviderComponent.h"
#include "QuestProviderPreferences.h"
#include "Components/ActorComponent.h"
#include "QuestCreationComponent.generated.h"

class UQuest;
class UQuestCreationComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestUpdated, FQuestGenerationSnapshot, Snapshot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOniterationUpdated, FQuestGenerationSnapshot, Snapshot);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable, BlueprintType)
class AESIRPROCEDURALQUEST_API UQuestCreationComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UQuestCreationComponent();

	UFUNCTION(BlueprintCallable)
	void Initialize();
	
	FGuid RequestQuestGeneration(UQuestProviderComponent* QuestProviderComponent);
	
	UQuest* FinishQuestGeneration(const FGuid& Id);

	UPROPERTY(BlueprintAssignable)
	FOnQuestUpdated OnQuestUpdated;

	UPROPERTY(BlueprintAssignable)
	FOnQuestUpdated OnQuestCompleted;

	UPROPERTY(BlueprintAssignable)
	FOniterationUpdated OnIterationUpdated;

protected:
	UPROPERTY(EditAnywhere)
	float MaxTickTime = 0.03f;

	void ProceedGeneration(FQuestGenerationSnapshot& Snapshot);
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	/**
	* DataTable that holds all possible quest actions
	* */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = ( RowType="QuestActionRow" ))
	UDataTable* QuestActionDataTable;

	UQuest* CreateRandomQuest(const uint32 QuestActionCount, UQuestProviderPreferences* Preferences);

	UQuest* MutateQuest(UQuest* BaseQuest, const int32 QuestActionCount, UQuestProviderPreferences* GenerationData);

	UQuest* MutateQuestByReplaceAction(UQuest* BaseQuest, const int32 QuestActionCount, UQuestProviderPreferences* GenerationData);

	UQuest* MutateQuestByScramblingActions(UQuest* BaseQuest, const int32 QuestActionCount, UQuestProviderPreferences* GenerationData);

	UPROPERTY(EditAnywhere)
	FInt32Range QuestActionCountRange = FInt32Range(5,10);

	UPROPERTY(EditAnywhere)
	int MaxQuestSampleCount = 30;

	UPROPERTY(EditAnywhere)
	int32 IterationsToAbandonLocalMaximum = 500;

	bool TryApplyRandomNextQuestAction(UQuest* Quest, TMap<uint32, uint32>& SimulatedConditionResolutions) const;

	bool TryApplyNextQuestAction(UQuest* Quest, UQuestAction* ActionCandidate, TMap<uint32, uint32>& SimulatedConditionResolutions) const;

	UQuestAction* GetRandomQuestAction(UObject* Outer = nullptr) const;

	void InitPossibleQuestActions();

	UPROPERTY(Transient)
	TArray<UQuestAction*> CachedPossibleQuestActions;

	TMap<FGuid, FQuestGenerationSnapshot> GenerationSnapshots;
};
