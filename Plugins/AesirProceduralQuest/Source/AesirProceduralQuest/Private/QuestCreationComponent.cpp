// Copyright 2022 Jan Plähn. All Rights Reserved.

#include "QuestCreationComponent.h"
#include "QuestProviderComponent.h"
#include "AesirProceduralQuest.h"
#include "AesirProceduralQuestBPLibrary.h"
#include "Quest.h"
#include "QuestActionDatabaseComponent.h"
#include "QuestFitnessBPLibrary.h"
#include "Kismet/GameplayStatics.h"

UQuestCreationComponent::UQuestCreationComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UQuestCreationComponent::InjectDependencies(UQuestActionDatabaseComponent* InActionDatabase)
{
	ActionDatabase = InActionDatabase;
}

//Todo: Only pass in UObject-Context and Preferences
FGuid UQuestCreationComponent::RequestQuestGeneration(UQuestProviderComponent* QuestProviderComponent)
{
	const FGuid& NewGuid = FGuid::NewGuid();
	const uint32 QuestActionCount = FMath::RandRange(QuestActionCountRange.GetLowerBound().GetValue(), QuestActionCountRange.GetUpperBound().GetValue());
	GenerationSnapshots.Add(NewGuid, FQuestGenerationSnapshot(QuestProviderComponent, QuestProviderComponent->GetPreferences(), QuestActionCount));
	return NewGuid;
}

UQuest* UQuestCreationComponent::FinishQuestGeneration(const FGuid& Id)
{
	const FQuestGenerationSnapshot& Snapshot = GenerationSnapshots.FindAndRemoveChecked(Id);
	OnQuestCompleted.Broadcast(Snapshot);
	ActionDatabase->CleanActionInstances();
	return Snapshot.GlobalMaximum;
}

//Todo: Move this function to the snapshot and generate utility functions for Mutation and Creation
void UQuestCreationComponent::ProceedGeneration(FQuestGenerationSnapshot& Snapshot) const
{
	Snapshot.TotalIterations++;
	Snapshot.IterationsSinceLastGlobalImprovement++;
	Snapshot.IterationsSinceLastLocalImprovement++;
	
	OnIterationUpdated.Broadcast(Snapshot);

	//Todo: Save the action count in the snapshot
	if (Snapshot.IterationsSinceLastLocalImprovement >= FMath::Max(MinLocalIterations, Snapshot.TotalIterations * AbandonBias))
	{
		Snapshot.LocalMaximum->ClearQuest();
		Snapshot.LocalMaximumFitness = -1;
		Snapshot.IterationsSinceLastLocalImprovement = 0;
	}

	if (Snapshot.LocalMaximum->IsEmpty())
	{
		AssignRandomActions(Snapshot.Candidate, Snapshot.ActionCount);
	}
	else
	{
		MutateQuest(Snapshot.Candidate, Snapshot.LocalMaximum);
	}

	//For debugging purposes only
	if (Snapshot.Candidate->IsEmpty())
	{
		Snapshot.NullQuestCount++;
		return;
	}
	//~For debugging purposes

	const float CandidateFitness = UQuestFitnessUtils::CalculateWeightedFitness(this, Snapshot.Candidate, Snapshot.GenerationData.Get());
	if (CandidateFitness > Snapshot.LocalMaximumFitness)
	{
		Snapshot.LocalMaximum->CopyFrom(Snapshot.Candidate);
		Snapshot.LocalMaximumFitness = CandidateFitness;
		Snapshot.IterationsSinceLastLocalImprovement = 0;
	}

	if (CandidateFitness > Snapshot.GlobalMaximumFitness)
	{
		Snapshot.GlobalMaximum->CopyFrom(Snapshot.Candidate);
		Snapshot.GlobalMaximumFitness = CandidateFitness;
		Snapshot.IterationsSinceLastGlobalImprovement = 0;
		OnQuestUpdated.Broadcast(Snapshot);
	}
}

void UQuestCreationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	const double StartTickTime = FPlatformTime::Seconds();
	while (FPlatformTime::Seconds() - StartTickTime < MaxTickTime)
	{
		for (auto& Kvp : GenerationSnapshots)
		{
			ProceedGeneration(Kvp.Value);
		}
	}	
}

void UQuestCreationComponent::AssignRandomActions(UQuest* InOutQuest, const uint32 Count) const
{
	InOutQuest->ClearQuest();

	TMap<uint32, uint32> SimulatedResolutions;
	for (uint32 QuestIndex = 0; QuestIndex < Count; QuestIndex++)
	{
		if (!TryApplyRandomNextQuestAction(InOutQuest, SimulatedResolutions))
		{
			InOutQuest->ClearQuest();
			return;
		}
	}
}

void UQuestCreationComponent::MutateQuest(UQuest* Mutation, UQuest* BaseQuest) const
{
	const int MutationIndex = FMath::RandRange(0,4);
	if (MutationIndex == 0)
	{
		MutateQuestByScramblingActions(Mutation, BaseQuest);
	}
	else
	{
		MutateQuestByReplaceAction(Mutation, BaseQuest);
	}
}

void UQuestCreationComponent::MutateQuestByReplaceAction(UQuest* Mutation, UQuest* BaseQuest) const
{
	const int32 QuestActionCount = BaseQuest->GetActions().Num();
	Mutation->ClearQuest();
	
	const int MutatedActionIndex = FMath::RandRange(0, BaseQuest->GetActions().Num()-1);

	TMap<uint32, uint32> SimulatedConditionResolutions;

	//Initialize first few actions just as they were
	for (int ActionIndex = 0; ActionIndex < MutatedActionIndex; ActionIndex++)
	{
		const TWeakObjectPtr<UQuestAction> Action = BaseQuest->GetActions()[ActionIndex];
		ensure(TryApplyNextQuestAction(Mutation, Action, SimulatedConditionResolutions));
	}

	//Apply random next action as mutation
	if (!TryApplyRandomNextQuestAction(Mutation, SimulatedConditionResolutions))
	{
		AssignRandomActions(Mutation, QuestActionCount);
		return;
	}

	if (MutatedActionIndex == BaseQuest->GetActions().Num()-1)
	{
		AssignRandomActions(Mutation, QuestActionCount);
		return;
	}

	//Initialize last few actions: Try to keep them the same, otherwise change them
	for (int ActionIndex = MutatedActionIndex+1; ActionIndex < BaseQuest->GetActions().Num(); ActionIndex++)
	{
		TWeakObjectPtr<UQuestAction> Action = BaseQuest->GetActions()[ActionIndex];
		if (TryApplyNextQuestAction(Mutation, Action, SimulatedConditionResolutions))
		{
			continue;
		}
		
		if (!TryApplyRandomNextQuestAction(Mutation, SimulatedConditionResolutions))
		{
			AssignRandomActions(Mutation, QuestActionCount);
			return;
		}
	}
}

void UQuestCreationComponent::MutateQuestByScramblingActions(UQuest* Mutation, UQuest* BaseQuest) const
{
	const int32 QuestActionCount = BaseQuest->GetActions().Num();
	Mutation->ClearQuest();
    
    const int SwapIndexA = FMath::RandRange(0, BaseQuest->GetActions().Num()-1);
	const int SwapIndexB = FMath::RandRange(0, BaseQuest->GetActions().Num()-1);
   
    TMap<uint32, uint32> SimulatedConditionResolutions;
   
    //Initialize first few actions just as they were
    for (int ActionIndex = 0; ActionIndex < BaseQuest->GetActions().Num(); ActionIndex++)
    {
	    const TWeakObjectPtr<UQuestAction> Action =
		    ActionIndex == SwapIndexA
			    ? BaseQuest->GetActions()[SwapIndexB]
			    : ActionIndex == SwapIndexB
			    ? BaseQuest->GetActions()[SwapIndexA]
			    : BaseQuest->GetActions()[ActionIndex];

    	if (TryApplyNextQuestAction(Mutation, Action, SimulatedConditionResolutions))
    	{
    		continue;
    	}
    	
    	if (!TryApplyRandomNextQuestAction(Mutation, SimulatedConditionResolutions))
    	{
    		AssignRandomActions(Mutation, QuestActionCount);
    		return;
    	}
    }
}

bool UQuestCreationComponent::TryApplyRandomNextQuestAction(UQuest* Quest, TMap<uint32, uint32>& SimulatedConditionResolutions) const
{
	for(int AttemptIndex = 0; AttemptIndex < MaxQuestSampleCount; AttemptIndex++)
	{
		UQuestAction* ActionCandidate = ActionDatabase->GetSuitableAction(Quest);

		if (TryApplyNextQuestAction(Quest, ActionCandidate, SimulatedConditionResolutions))
		{
			return true;
		}
	}
	return false;
}

bool UQuestCreationComponent::TryApplyNextQuestAction(UQuest* Quest, TWeakObjectPtr<UQuestAction> ActionCandidate,
	TMap<uint32, uint32>& SimulatedConditionResolutions) const
{
	if (!ActionCandidate->SimulateIsAvailable(this, SimulatedConditionResolutions))
	{
		return false;
	}

	if (Quest->GetActions().ContainsByPredicate([&ActionCandidate](const TWeakObjectPtr<UQuestAction> Action){return Action->GetId() == ActionCandidate->GetId();}))
	{
		return false;
	}

	Quest->AddQuestAction(ActionCandidate);
	for (const UQuestCondition* Condition : ActionCandidate->GetPostConditions())
	{
		const uint32 Id = Condition->GetId();
		uint32 Resolution = 0;

		switch (Condition->GetConditionType())
		{
			default:
			case EConditionType::Boolean:
				{
					Resolution = Condition->bInvertCondition ? false : true;
					break;
				}
			case EConditionType::State:
				{
					Resolution = Condition->GetStateId();
					break;
				}
		}
		
		SimulatedConditionResolutions.Add(Id, Resolution);
	}
	return true;
}

