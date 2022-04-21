// Copyright 2022 Jan Plähn. All Rights Reserved.

#include "QuestCreationComponent.h"
#include "QuestProviderComponent.h"
#include "AesirProceduralQuest.h"
#include "AesirProceduralQuestBPLibrary.h"
#include "Quest.h"
#include "QuestActionRow.h"
#include "QuestFitnessBPLibrary.h"
#include "Kismet/GameplayStatics.h"

UQuestCreationComponent::UQuestCreationComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UQuestCreationComponent::Initialize()
{
	InitPossibleQuestActions();
}

//Todo: Only pass in UObject-Context and Preferences
FGuid UQuestCreationComponent::RequestQuestGeneration(UQuestProviderComponent* QuestProviderComponent)
{
	const FGuid& NewGuid = FGuid::NewGuid();
	GenerationSnapshots.Add(NewGuid, FQuestGenerationSnapshot(QuestProviderComponent, QuestProviderComponent->GetPreferences()));
	return NewGuid;
}

UQuest* UQuestCreationComponent::FinishQuestGeneration(const FGuid& Id)
{
	const FQuestGenerationSnapshot& Snapshot = GenerationSnapshots.FindAndRemoveChecked(Id);
	OnQuestCompleted.Broadcast(Snapshot);
	GEngine->ForceGarbageCollection(true);
	return Snapshot.GlobalMaximum;
}

void UQuestCreationComponent::ProceedGeneration(FQuestGenerationSnapshot& Snapshot)
{
	Snapshot.TotalIterations++;
	Snapshot.IterationsSinceLastGlobalImprovement++;
	Snapshot.IterationsSinceLastLocalImprovement++;
	
	OnIterationUpdated.Broadcast(Snapshot);

	//Todo: Save the action count in the snapshot
	const int32 QuestActionCount = !Snapshot.GlobalMaximum->IsEmpty() ? Snapshot.GlobalMaximum->GetActions().Num() : FMath::RandRange(QuestActionCountRange.GetLowerBound().GetValue(), QuestActionCountRange.GetUpperBound().GetValue());
	if (Snapshot.IterationsSinceLastLocalImprovement >= FMath::Max(MinLocalIterations, Snapshot.TotalIterations * AbandonBias))
	{
		Snapshot.LocalMaximum->Clear();
		AssignRandomActions(Snapshot.Candidate, QuestActionCount);
		Snapshot.IterationsSinceLastLocalImprovement = 0;
	}
	else
	{
		MutateQuest(Snapshot.Candidate, Snapshot.LocalMaximum, QuestActionCount, Snapshot.GenerationData.Get());
	}
	if (Snapshot.Candidate->IsEmpty())
	{
		Snapshot.NullQuestCount++;
		return;
	}
	UQuest* NewLocalMaximum = UQuestFitnessUtils::SelectFittest(this, Snapshot.LocalMaximum, Snapshot.Candidate, Snapshot.GenerationData.Get());

	if (Snapshot.LocalMaximum->CopyFrom(NewLocalMaximum))
	{
		Snapshot.IterationsSinceLastLocalImprovement = 0;
	}
	
	UQuest* NewGlobalMaximum = UQuestFitnessUtils::SelectFittest(this, Snapshot.GlobalMaximum, NewLocalMaximum, Snapshot.GenerationData.Get());
	
	if (Snapshot.GlobalMaximum->CopyFrom(NewGlobalMaximum))
	{
		UE_LOG(LogProceduralQuests, Verbose, TEXT("GLOBAL MAXIMUM | Current Fitness: [%f]."), UQuestFitnessUtils::CalculateWeightedFitness(this, NewGlobalMaximum, Snapshot.GenerationData.Get()));
		OnQuestUpdated.Broadcast(Snapshot);
		Snapshot.IterationsSinceLastGlobalImprovement = 0;
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
	InOutQuest->Clear();

	TMap<uint32, uint32> SimulatedResolutions;
	for (uint32 QuestIndex = 0; QuestIndex < Count; QuestIndex++)
	{
		if (!ensure(TryApplyRandomNextQuestAction(InOutQuest, SimulatedResolutions)))
		{
			InOutQuest->Clear();
			return;
		}
	}

	ensure(InOutQuest->GetActions().Num() != 1);
}

void UQuestCreationComponent::MutateQuest(UQuest* InOutQuest, UQuest* BaseQuest, const int32 QuestActionCount, UQuestProviderPreferences* GenerationData)
{
	const int MutationIndex = FMath::RandRange(0,4);
	if (MutationIndex == 0)
	{
		MutateQuestByScramblingActions(InOutQuest, BaseQuest, QuestActionCount, GenerationData);
	}
	else
	{
		MutateQuestByReplaceAction(InOutQuest, BaseQuest,QuestActionCount, GenerationData);
	}
	ensure(InOutQuest->GetActions().Num() != 1);
}

void UQuestCreationComponent::MutateQuestByReplaceAction(UQuest* InOutQuest, UQuest* BaseQuest, const int32 QuestActionCount, UQuestProviderPreferences* GenerationData)
{
	if (BaseQuest->IsEmpty())
	{
		AssignRandomActions(InOutQuest, QuestActionCount);
		return;
	}

	InOutQuest->Clear();
	
	const int MutatedActionIndex = FMath::RandRange(0, BaseQuest->GetActions().Num()-1);

	TMap<uint32, uint32> SimulatedConditionResolutions;

	//Initialize first few actions just as they were
	for (int ActionIndex = 0; ActionIndex < MutatedActionIndex; ActionIndex++)
	{
		const UQuestAction* BaseAction = BaseQuest->GetActions()[ActionIndex];
		UQuestAction* DuplicateAction = DuplicateObject(BaseAction, InOutQuest);
		DuplicateAction->InitializeAsInstance();
		ensure(TryApplyNextQuestAction(InOutQuest, DuplicateAction, SimulatedConditionResolutions));
	}

	//Apply random next action as mutation
	if (!TryApplyRandomNextQuestAction(InOutQuest, SimulatedConditionResolutions))
	{
		AssignRandomActions(InOutQuest, QuestActionCount);
		return;
	}

	if (MutatedActionIndex == BaseQuest->GetActions().Num()-1)
	{
		AssignRandomActions(InOutQuest, QuestActionCount);
		return;
	}

	//Initialize last few actions: Try to keep them the same, otherwise change them
	for (int ActionIndex = MutatedActionIndex+1; ActionIndex < BaseQuest->GetActions().Num(); ActionIndex++)
	{
		const UQuestAction* BaseAction = BaseQuest->GetActions()[ActionIndex];

		//Todo: Don't duplicate but store this in some database!
		UQuestAction* DuplicateAction = DuplicateObject(BaseAction, InOutQuest);
		DuplicateAction->InitializeAsInstance();
		if (TryApplyNextQuestAction(InOutQuest, DuplicateAction, SimulatedConditionResolutions))
		{
			continue;
		}

		DuplicateAction->ConditionalBeginDestroy();
		DuplicateAction = nullptr;
		
		if (!TryApplyRandomNextQuestAction(InOutQuest, SimulatedConditionResolutions))
		{
			AssignRandomActions(InOutQuest, QuestActionCount);
			return;
		}
	}
}

void UQuestCreationComponent::MutateQuestByScramblingActions(UQuest* InOutQuest, UQuest* BaseQuest, const int32 QuestActionCount, UQuestProviderPreferences* GenerationData)
{
	InOutQuest->Clear();
	if (BaseQuest->IsEmpty())
    {
		AssignRandomActions(InOutQuest, QuestActionCount);
		return;
    }
    
    const int SwapIndexA = FMath::RandRange(0, BaseQuest->GetActions().Num()-1);
	const int SwapIndexB = FMath::RandRange(0, BaseQuest->GetActions().Num()-1);
   
    TMap<uint32, uint32> SimulatedConditionResolutions;
   
    //Initialize first few actions just as they were
    for (int ActionIndex = 0; ActionIndex < BaseQuest->GetActions().Num(); ActionIndex++)
    {
    	const UQuestAction* BaseAction =
    		ActionIndex == SwapIndexA ? BaseQuest->GetActions()[SwapIndexB] :
    		ActionIndex == SwapIndexB ? BaseQuest->GetActions()[SwapIndexA] :   	
    		BaseQuest->GetActions()[ActionIndex];

    	//Todo: Save these actions somewhere else (e.g. database!)
    	UQuestAction* DuplicateAction = DuplicateObject(BaseAction, InOutQuest);
    	DuplicateAction->InitializeAsInstance();
    	if (TryApplyNextQuestAction(InOutQuest, DuplicateAction, SimulatedConditionResolutions))
    	{
    		continue;
    	}
		DuplicateAction->ConditionalBeginDestroy();
    	DuplicateAction = nullptr;
    	
    	if (!TryApplyRandomNextQuestAction(InOutQuest, SimulatedConditionResolutions))
    	{
    		AssignRandomActions(InOutQuest, QuestActionCount);
    		return;
    	}
    }
}

bool UQuestCreationComponent::TryApplyRandomNextQuestAction(UQuest* Quest, TMap<uint32, uint32>& SimulatedConditionResolutions) const
{
	for(int AttemptIndex = 0; AttemptIndex < MaxQuestSampleCount; AttemptIndex++)
	{
		UQuestAction* ActionCandidate = GetRandomQuestAction(Quest);

		if (TryApplyNextQuestAction(Quest, ActionCandidate, SimulatedConditionResolutions))
		{
			return true;
		}

		ActionCandidate->ConditionalBeginDestroy();
		ActionCandidate = nullptr;
	}
	return false;
}

bool UQuestCreationComponent::TryApplyNextQuestAction(UQuest* Quest, UQuestAction* ActionCandidate,
	TMap<uint32, uint32>& SimulatedConditionResolutions) const
{
	if (!ActionCandidate->SimulateIsAvailable(this, SimulatedConditionResolutions))
	{
		return false;
	}

	if (Quest->GetActions().ContainsByPredicate([&ActionCandidate](const UQuestAction* Action){return Action->GetId() == ActionCandidate->GetId();}))
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

UQuestAction* UQuestCreationComponent::GetRandomQuestAction(UObject* Outer) const
{
	const int TotalActionCount = CachedPossibleQuestActions.Num();
	if (!ensureMsgf(TotalActionCount > 0, TEXT("QuestCreationComponent: Could not find cached quests.")))
	{
		return nullptr;
	}
	
	const int RandomIndex = FMath::RandRange(0, TotalActionCount - 1);
	return CachedPossibleQuestActions[RandomIndex]->MakeRandomInstance(Outer);
}

void UQuestCreationComponent::InitPossibleQuestActions()
{
	UE_LOG(LogProceduralQuests, Verbose, TEXT("Caching %d possible quest actions"), QuestActionDataTable->GetRowNames().Num());

	if (!IsValid(QuestActionDataTable))
		return;

	if (QuestActionDataTable->GetRowNames().Num() <= 0)
		return;
	
	QuestActionDataTable->ForeachRow<FQuestActionRow>("UQuestCreationComponent::InitPossibleQuestActions",
		[this](const FName& RowName, const FQuestActionRow& Row)
		{

			TSet<UQuestCondition*> Conditions;
			Conditions.Append(Row.QuestAction->GetPreConditions());
			Conditions.Append(Row.QuestAction->GetPostConditions());
			CachedPossibleQuestActions.Add(Row.QuestAction);
		}
	);
}

