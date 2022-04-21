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
	OnQuestCompleted.Broadcast(QuestProviderComponent->GetQuest());
}

int UQuestCreationComponent::GetTotalIterations() const
{
	return TotalIterations;
}

int UQuestCreationComponent::GetLocalIterationIndex() const
{
	return IterationsSinceLastLocalImprovement;
}

int UQuestCreationComponent::GetNullQuestCount() const
{
	return NullQuestCount;
}

void UQuestCreationComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                            FActorComponentTickFunction* ThisTickFunction)
{

	const double StartTickTime = FPlatformTime::Seconds();
	while (FPlatformTime::Seconds() - StartTickTime < MaxTickTime)
	{
		for (UQuestProviderComponent* Provider : QuestRequesters)
		{
			TotalIterations++;
			IterationsSinceLastGlobalImprovement++;
			IterationsSinceLastLocalImprovement++;
			OnIterationUpdated.Broadcast(this);
			UQuest* GlobalMaximumQuest = Provider->GetQuest();
			const int32 QuestActionCount = IsValid(GlobalMaximumQuest) ? GlobalMaximumQuest->GetActions().Num() : FMath::RandRange(QuestActionCountRange.GetLowerBound().GetValue(), QuestActionCountRange.GetUpperBound().GetValue());
			UQuest* NewQuest;
			if (IterationsSinceLastLocalImprovement >= TotalIterations / 4)
			{
				LocalMaximumQuest = nullptr;
				//UE_LOG(LogProceduralQuests, Verbose, TEXT("LOCAL MAXIMUM REACHED after %d local iterations."), static_cast<float>(FPlatformTime::Seconds() - StartTimestamp), IterationsSinceLastLocalImprovementImprovement);

				NewQuest = CreateRandomQuest(QuestActionCount, Provider);
				IterationsSinceLastLocalImprovement = 0;
			}
			else
			{
				NewQuest = MutateQuest(LocalMaximumQuest.Get(), QuestActionCount, Provider);
			}
			if (!IsValid(NewQuest))
			{
				NullQuestCount++;
				continue;
			}
			NewQuest->SetProviderData(Provider->GetPreferences());
			UQuest* NewLocalMaximum = UQuestFitnessUtils::SelectFittest(this, LocalMaximumQuest.Get(), NewQuest, Provider->GetPreferences());

			if (NewQuest != NewLocalMaximum)
			{
				NewQuest->ConditionalBeginDestroy();
				NewQuest = nullptr;
			}
			else
			{
				if (LocalMaximumQuest.IsValid())
				{
					LocalMaximumQuest->ConditionalBeginDestroy();
					LocalMaximumQuest.Reset();
				}
			}
			LocalMaximumQuest = NewLocalMaximum;

			UQuest* NewGlobalMaximum = UQuestFitnessUtils::SelectFittest(this, GlobalMaximumQuest, NewLocalMaximum, Provider->GetPreferences());
			if (NewQuest == NewGlobalMaximum)
			{
				UE_LOG(LogProceduralQuests, Verbose, TEXT("GLOBAL MAXIMUM | Timestamp: %f | Current Fitness: [%f] after %d global iterations."), static_cast<float>(FPlatformTime::Seconds() - StartTimestamp), UQuestFitnessUtils::CalculateWeightedFitness(this, NewGlobalMaximum, Provider->GetPreferences()), IterationsSinceLastGlobalImprovement);
				OnQuestUpdated.Broadcast(NewGlobalMaximum);

				IterationsSinceLastGlobalImprovement = 0;
				if (IsValid(GlobalMaximumQuest))
				{
					GlobalMaximumQuest->ConditionalBeginDestroy();
					GlobalMaximumQuest = nullptr;
				}
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

			if (NewGlobalMaximum)

			Provider->SetQuest(NewGlobalMaximum);		
		}
	}	
}

UQuest* UQuestCreationComponent::CreateRandomQuest(const uint32 QuestActionCount, const UQuestProviderComponent * ProviderComponent)
{	
	if (!ensureMsgf(CachedPossibleQuestActions.Num() != 0, TEXT("Quest actions have not been initialized")))
		return nullptr;

	TMap<uint32, uint32> SimulatedConditionResolutions;
	UQuest* RandomQuest = NewObject<UQuest>(this);
	RandomQuest->SetProviderData(ProviderComponent->GetPreferences());
	for (uint32 QuestIndex = 0; QuestIndex < QuestActionCount; QuestIndex++)
	{
		if (!TryApplyRandomNextQuestAction(RandomQuest, SimulatedConditionResolutions))
		{
			RandomQuest->ConditionalBeginDestroy();
			return nullptr;
		}
	}
	
	return RandomQuest;
}

UQuest* UQuestCreationComponent::MutateQuest(UQuest* BaseQuest, const int32 QuestActionCount, const UQuestProviderComponent * ProviderComponent)
{
	const int MutationIndex = FMath::RandRange(0,4);
	if (MutationIndex == 0)
	{
		return MutateQuestByScramblingActions(BaseQuest, QuestActionCount, ProviderComponent);
	}
	else
	{
		return MutateQuestByReplaceAction(BaseQuest,QuestActionCount, ProviderComponent);
	}
}

UQuest* UQuestCreationComponent::MutateQuestByReplaceAction(UQuest* BaseQuest, const int32 QuestActionCount, const UQuestProviderComponent * ProviderComponent)
{
	if (!IsValid(BaseQuest))
	{
		return CreateRandomQuest(QuestActionCount, ProviderComponent);
	}
	UQuest* NewQuest = NewObject<UQuest>(this);
	NewQuest->SetProviderData(ProviderComponent->GetPreferences());
	
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

UQuest* UQuestCreationComponent::MutateQuestByScramblingActions(UQuest* BaseQuest, const int32 QuestActionCount, const UQuestProviderComponent* ProviderComponent)
{
	if (!IsValid(BaseQuest))
    {
		return CreateRandomQuest(QuestActionCount, ProviderComponent);
    }
    UQuest* NewQuest = NewObject<UQuest>(this);
	NewQuest->SetProviderData(ProviderComponent->GetPreferences());
    
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

bool UQuestCreationComponent::TryApplyRandomNextQuestAction(UQuest* Quest, TMap<uint32, uint32>& SimulatedConditionResolutions) const
{
	for(int AttemptIndex = 0; AttemptIndex < MaxQuestSampleCount; AttemptIndex++)
	{
		UQuestAction* ActionCandidate = GetRandomQuestAction(Quest);

		if (TryApplyNextQuestAction(Quest, ActionCandidate, SimulatedConditionResolutions))
		{
			return true;
		}

		ActionCandidate->MarkPendingKill();
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

