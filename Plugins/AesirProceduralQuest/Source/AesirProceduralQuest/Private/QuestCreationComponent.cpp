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

void UQuestCreationComponent::RequestQuestGeneration(UQuestProviderComponent* QuestProviderComponent)
{
	QuestRequesters.Add(QuestProviderComponent);
	StartTimestamp = LastLogTimestamp = FPlatformTime::Seconds();
	IterationsSinceLastGlobalImprovement = 0;
}

void UQuestCreationComponent::PauseQuestGeneration(UQuestProviderComponent* QuestProviderComponent)
{
	UE_LOG(LogProceduralQuests, Verbose, TEXT("FINAL MAXIMUM | Timestamp: %f | Current Fitness: [%f] after %d global iterations."), static_cast<float>(FPlatformTime::Seconds() - StartTimestamp), UQuestFitnessUtils::CalculateWeightedFitness(this, QuestProviderComponent->GetQuest(),  QuestProviderComponent->GetPreferences()), IterationsSinceLastGlobalImprovement);
	IterationsSinceLastGlobalImprovement = 0;
	QuestRequesters.Remove(QuestProviderComponent);
}

void UQuestCreationComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	
	//Todo: Maybe do this with a timer instead (do as many iterations as possible in 1ms or so)
	for (int GenerationIteration = 0; GenerationIteration < GenerationIterationsPerFrame; GenerationIteration++)
	{
		for (UQuestProviderComponent* Provider : QuestRequesters)
		{
			IterationsSinceLastGlobalImprovement++;
			IterationsSinceLastLocalImprovement++;
			UQuest* GlobalMaximumQuest = Provider->GetQuest();
			UQuest* NewQuest;
			if (IterationsSinceLastLocalImprovement >= IterationsToAbandonLocalMaximum)
			{
				LocalMaximumQuest = nullptr;
				//UE_LOG(LogProceduralQuests, Verbose, TEXT("LOCAL MAXIMUM REACHED after %d local iterations."), static_cast<float>(FPlatformTime::Seconds() - StartTimestamp), IterationsSinceLastLocalImprovementImprovement);
				NewQuest = CreateRandomQuest();
				IterationsSinceLastLocalImprovement = 0;
			}
			else
			{
				NewQuest = MutateQuest(LocalMaximumQuest.Get());
			}
			if (!IsValid(NewQuest))
			{
				continue;
			}
			NewQuest->SetProviderData(Provider->GetPreferences());
			UQuest* NewLocalMaximum = UQuestFitnessUtils::SelectFittest(this, LocalMaximumQuest.Get(), NewQuest, Provider->GetPreferences());
			// if (UQuestFitnessUtils::CalculateWeightedFitness(this, NewLocalMaximum, Provider->GetPreferences()) < UQuestFitnessUtils::CalculateWeightedFitness(this, LocalMaximumQuest.Get(), Provider->GetPreferences()))
			// {
			// 	UE_LOG(LogProceduralQuests, Verbose, TEXT("!?"));			
			// }


			if (NewQuest != NewLocalMaximum)
			{
				NewQuest->ConditionalBeginDestroy();
				NewQuest = nullptr;
			}
			else
			{
				// if (LocalMaximumQuest.IsValid())
				// {
				// 	LocalMaximumQuest->MarkPendingKill();
				// }
			}
			LocalMaximumQuest = NewLocalMaximum;

			UQuest* NewGlobalMaximum = UQuestFitnessUtils::SelectFittest(this, GlobalMaximumQuest, NewLocalMaximum, Provider->GetPreferences());
			if (NewQuest == NewGlobalMaximum)
			{
				UE_LOG(LogProceduralQuests, Verbose, TEXT("GLOBAL MAXIMUM | Timestamp: %f | Current Fitness: [%f] after %d global iterations."), static_cast<float>(FPlatformTime::Seconds() - StartTimestamp), UQuestFitnessUtils::CalculateWeightedFitness(this, NewGlobalMaximum, Provider->GetPreferences()), IterationsSinceLastGlobalImprovement);
				IterationsSinceLastGlobalImprovement = 0;
			}
			else if (NewQuest == NewLocalMaximum)
			{
				//UE_LOG(LogProceduralQuests, Verbose, TEXT("LOCAL MAXIMUM | Timestamp: %f | Current Fitness: [%f] after %d local iterations."), static_cast<float>(FPlatformTime::Seconds() - StartTimestamp), UQuestFitnessUtils::CalculateFitnessByDesiredConditions(this, NewLocalMaximum, Provider->GetPreferences()),  UQuestFitnessUtils::CalculateWeightedFitness(this, NewLocalMaximum, Provider->GetPreferences()), IterationsSinceLastLocalImprovement);
				IterationsSinceLastLocalImprovement = 0;
			}

			if (UQuestFitnessUtils::CalculateWeightedFitness(this, NewGlobalMaximum, Provider->GetPreferences()) < UQuestFitnessUtils::CalculateWeightedFitness(this, GlobalMaximumQuest, Provider->GetPreferences()))
			{
				UE_LOG(LogProceduralQuests, Verbose, TEXT("SOMETHING IS WRONG!"));			
			}

			Provider->SetQuest(NewGlobalMaximum);
	

			// const double LogInterval = 0.001;
			// if (FPlatformTime::Seconds() - LastLogTimestamp > LogInterval)
			// {
			// 	UE_LOG(LogProceduralQuests, Verbose, TEXT("Current Fitness: [%f (Primary)] [%f (Secondary)]"),UQuestFitnessUtils::CalculateFitnessByDesiredConditions(this, SelectedQuest, Provider->GetPreferences()),  UQuestFitnessUtils::CalculateWeightedFitness(this, SelectedQuest, Provider->GetPreferences()));
			// 	LastLogTimestamp = FPlatformTime::Seconds() - (FPlatformTime::Seconds() - LastLogTimestamp - LogInterval);
			// }
			
		}
	}	
}

UQuest* UQuestCreationComponent::CreateRandomQuest()
{	
	if (!ensureMsgf(CachedPossibleQuestActions.Num() != 0, TEXT("Quest actions have not been initialized")))
		return nullptr;

	TMap<uint32, uint32> SimulatedConditionResolutions;
	UQuest* RandomQuest = NewObject<UQuest>(this);
	const int32 QuestActionCount = FMath::RandRange(QuestActionCountRange.GetLowerBound().GetValue(), QuestActionCountRange.GetUpperBound().GetValue());
	for (int32 QuestIndex = 0; QuestIndex < QuestActionCount; QuestIndex++)
	{
		if (!TryApplyRandomNextQuestAction(RandomQuest, SimulatedConditionResolutions))
		{
			RandomQuest->ConditionalBeginDestroy();
			return nullptr;
		}
	}
	
	return RandomQuest;
}

UQuest* UQuestCreationComponent::MutateQuest(UQuest* BaseQuest)
{
	int MutationIndex = FMath::RandRange(0,1);
	if (MutationIndex == 0)
	{
		return MutateQuestByReplaceAction(BaseQuest);
	}
	else //if (MutationIndex == 1)
	{
		return MutateQuestByScramblingActions(BaseQuest);
	}
	// else
	// {
	// 	return MutateQuestByChangingSize(BaseQuest);
	// }
}

UQuest* UQuestCreationComponent::MutateQuestByReplaceAction(UQuest* BaseQuest)
{
	if (!IsValid(BaseQuest))
	{
		return CreateRandomQuest();
	}
	UQuest* NewQuest = NewObject<UQuest>(this);
	
	const int MutatedActionIndex = FMath::RandRange(0, BaseQuest->GetActions().Num()-1);

	TMap<uint32, uint32> SimulatedConditionResolutions;

	//Initialize first few actions just as they were
	for (int ActionIndex = 0; ActionIndex < MutatedActionIndex; ActionIndex++)
	{
		const UQuestAction* BaseAction = BaseQuest->GetActions()[ActionIndex];
		UQuestAction* DuplicateAction = DuplicateObject(BaseAction, NewQuest);
		DuplicateAction->InitializeAsInstance();
		TryApplyNextQuestAction(NewQuest, DuplicateAction, SimulatedConditionResolutions);
	}

	//Apply random next action as mutation
	if (!TryApplyRandomNextQuestAction(NewQuest, SimulatedConditionResolutions))
	{
		return nullptr;
	}

	if (MutatedActionIndex == BaseQuest->GetActions().Num()-1)
	{
		return NewQuest;
	}

	//Initialize last few actions: Try to keep them the same, otherwise change them
	for (int ActionIndex = MutatedActionIndex+1; ActionIndex < BaseQuest->GetActions().Num(); ActionIndex++)
	{
		const UQuestAction* BaseAction = BaseQuest->GetActions()[ActionIndex];
		UQuestAction* DuplicateAction = DuplicateObject(BaseAction, NewQuest);
		DuplicateAction->InitializeAsInstance();
		if (TryApplyNextQuestAction(NewQuest, DuplicateAction, SimulatedConditionResolutions))
		{
			continue;
		}
		if (!TryApplyRandomNextQuestAction(NewQuest, SimulatedConditionResolutions))
		{
			DuplicateAction->ConditionalBeginDestroy();
			DuplicateAction = nullptr;
			return nullptr;
		}
	}

	return NewQuest;
}

UQuest* UQuestCreationComponent::MutateQuestByScramblingActions(UQuest* BaseQuest)
{
	if (!IsValid(BaseQuest))
    {
    	return CreateRandomQuest();
    }
    UQuest* NewQuest = NewObject<UQuest>(this);
    
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
    	UQuestAction* DuplicateAction = DuplicateObject(BaseAction, NewQuest);
    	DuplicateAction->InitializeAsInstance();
    	if (TryApplyNextQuestAction(NewQuest, DuplicateAction, SimulatedConditionResolutions))
    	{
    		continue;
    	}
    	if (!TryApplyRandomNextQuestAction(NewQuest, SimulatedConditionResolutions))
    	{
    		return nullptr;
    	}
    }
   
    return NewQuest;
}

UQuest* UQuestCreationComponent::MutateQuestByChangingSize(UQuest* BaseQuest)
{
	if (!IsValid(BaseQuest))
	{
		return CreateRandomQuest();
	}
	UQuest* NewQuest = NewObject<UQuest>(this);
    
	const int NewSize = FMath::RandRange(QuestActionCountRange.GetLowerBoundValue(), QuestActionCountRange.GetUpperBoundValue());
   
	TMap<uint32, uint32> SimulatedConditionResolutions;
   
	//Initialize first few actions just as they were
	for (int ActionIndex = 0; ActionIndex < NewSize; ActionIndex++)
	{
		if (ActionIndex >= BaseQuest->GetActions().Num())
		{
			if (!TryApplyRandomNextQuestAction(NewQuest, SimulatedConditionResolutions))
			{
				return nullptr;
			}
		}
		else
		{
			const UQuestAction* BaseAction = BaseQuest->GetActions()[ActionIndex];
			UQuestAction* DuplicateAction = DuplicateObject(BaseAction, NewQuest);
			if (TryApplyNextQuestAction(NewQuest, DuplicateAction, SimulatedConditionResolutions))
			{
				continue;
			}
			if (!TryApplyRandomNextQuestAction(NewQuest, SimulatedConditionResolutions))
			{
				return nullptr;
			}
		}
	}
   
	return NewQuest;
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

