// Copyright 2022 Jan Plähn. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "QuestGenerationSnapshot.h"
#include "QuestProviderComponent.h"
#include "Components/ActorComponent.h"
#include "QuestCreationComponent.generated.h"

class UQuestActionDatabaseComponent;
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
	void InjectDependencies(UQuestActionDatabaseComponent* InActionDatabase);
	
	FGuid RequestQuestGeneration(UObject* Owner, UQuestProviderPreferences* Preferences);
	
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

	UPROPERTY(EditAnywhere)
	float MinLocalIterations = 100;

	void ProceedGeneration(FQuestGenerationSnapshot& Snapshot) const;
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Todo: Move this to a quest creation library
	void AssignRandomActions(UQuest* InOutQuest, const uint32 Count) const;

	//Todo: Move this to a quest creation library
	void MutateQuest(UQuest* Mutation, UQuest* BaseQuest) const;

	//Todo: Move this to a quest creation library
	void MutateQuestByReplaceAction(UQuest* Mutation, UQuest* BaseQuest) const;

	//Todo: Move this to a quest creation library
	void MutateQuestByScramblingActions(UQuest* Mutation, UQuest* BaseQuest) const;

	UPROPERTY(EditAnywhere)
	FInt32Range QuestActionCountRange = FInt32Range(5,10);

	UPROPERTY(EditAnywhere)
	int MaxQuestSampleCount = 100;

	UPROPERTY(EditAnywhere)
	float AbandonBias = 0.1f; //This could be renamed? (It is very confusing)

	//Todo: Move this to a quest creation library
	bool TryApplyRandomNextQuestAction(UQuest* Quest, TMap<uint32, uint32>& SimulatedConditionResolutions) const;

	//Todo: Move this to a quest creation library
	bool TryApplyNextQuestAction(UQuest* Quest, TWeakObjectPtr<UQuestAction> ActionCandidate, TMap<uint32, uint32>& SimulatedConditionResolutions) const;

	UPROPERTY(Transient)
	TMap<FGuid, FQuestGenerationSnapshot> GenerationSnapshots;

	TWeakObjectPtr<UQuestActionDatabaseComponent> ActionDatabase;
};
