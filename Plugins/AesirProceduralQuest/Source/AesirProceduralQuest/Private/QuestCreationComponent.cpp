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
	UE_LOG(LogProceduralQuests, Verbose, TEXT("FINAL MAXIMUM | Timestamp: %f | Current Fitness: [%f] after %d global iterations."), static_cast<float>(FPlatformTime::Seconds() - StartTimestamp), UQuestFitnessUtils::CalculateWeightedFitness(this, QuestProviderComponent->GetQuest().Get(),  QuestProviderComponent->GetPreferences()), IterationsSinceLastGlobalImprovement);
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
			TSoftObjectPtr<UQuest> GlobalMaximumQuest = Provider->GetQuest();
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
			if (NewQuest != NewLocalMaximum)
			{
				NewQuest->MarkPendingKill();
			}
			else
			{
				if (LocalMaximumQuest.IsValid())
				{
					LocalMaximumQuest->MarkPendingKill();
				}
			}
			LocalMaximumQuest = NewLocalMaximum;

			UQuest* NewGlobalMaximum = UQuestFitnessUtils::SelectFittest(this, GlobalMaximumQuest.Get(), LocalMaximumQuest.Get(), Provider->GetPreferences());
			if (NewQuest == NewGlobalMaximum)
			{
				UE_LOG(LogProceduralQuests, Verbose, TEXT("GLOBAL MAXIMUM | Timestamp: %f | Current Fitness: [%f] after %d global iterations."), static_cast<float>(FPlatformTime::Seconds() - StartTimestamp), UQuestFitnessUtils::CalculateWeightedFitness(this, NewLocalMaximum, Provider->GetPreferences()), IterationsSinceLastGlobalImprovement);
				IterationsSinceLastGlobalImprovement = 0;
			}
			else if (NewQuest == NewLocalMaximum)
			{
				//UE_LOG(LogProceduralQuests, Verbose, TEXT("LOCAL MAXIMUM | Timestamp: %f | Current Fitness: [%f] after %d local iterations."), static_cast<float>(FPlatformTime::Seconds() - StartTimestamp), UQuestFitnessUtils::CalculateFitnessByDesiredConditions(this, NewLocalMaximum, Provider->GetPreferences()),  UQuestFitnessUtils::CalculateWeightedFitness(this, NewLocalMaximum, Provider->GetPreferences()), IterationsSinceLastLocalImprovement);
				IterationsSinceLastLocalImprovement = 0;
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

	TMap<uint32, bool> SimulatedConditionResolutions;
	UQuest* RandomQuest = NewObject<UQuest>(this);
	const int32 QuestActionCount = FMath::RandRange(QuestActionCountRange.GetLowerBound().GetValue(), QuestActionCountRange.GetUpperBound().GetValue());
	for (int32 QuestIndex = 0; QuestIndex < QuestActionCount; QuestIndex++)
	{
		if (!TryApplyRandomNextQuestAction(RandomQuest, SimulatedConditionResolutions))
		{
			RandomQuest->MarkPendingKill();
			return nullptr;
		}
	}
	
	return RandomQuest;
}

UQuest* UQuestCreationComponent::MutateQuest(UQuest* BaseQuest)
{
	if (!IsValid(BaseQuest))
	{
		return CreateRandomQuest();
	}
	UQuest* NewQuest = NewObject<UQuest>(this);
	
	const int MutatedActionIndex = FMath::RandRange(0, BaseQuest->GetActions().Num()-1);

	TMap<uint32, bool> SimulatedConditionResolutions;

	//Initialize first few actions just as they were
	for (int ActionIndex = 0; ActionIndex < MutatedActionIndex; ActionIndex++)
	{
		const UQuestAction* BaseAction = BaseQuest->GetActions()[ActionIndex];
		UQuestAction* DuplicateAction = DuplicateObject(BaseAction, NewQuest);
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
		if (TryApplyNextQuestAction(NewQuest, DuplicateObject(BaseAction, NewQuest), SimulatedConditionResolutions))
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

bool UQuestCreationComponent::TryApplyRandomNextQuestAction(UQuest* Quest, TMap<uint32, bool>& SimulatedConditionResolutions) const
{
	for(int AttemptIndex = 0; AttemptIndex < MaxQuestSampleCount; AttemptIndex++)
	{
		UQuestAction* ActionCandidate = GetRandomQuestAction(Quest);
		
		if (!ActionCandidate->SimulateIsAvailable(this, SimulatedConditionResolutions))
		{
			ActionCandidate->MarkPendingKill();
			continue;
		}
		
		Quest->AddQuestAction(ActionCandidate);
		for (const UQuestCondition* Condition : ActionCandidate->GetPostConditions())
		{
			if (Condition->GetName().StartsWith("HasItem") && !Condition->bInvertCondition)
			{
				int x = 0;
			}
			const uint32 Id = Condition->GetId();
			SimulatedConditionResolutions.Add(Id, !Condition->bInvertCondition);
		}
		return true;
	}
	return false;
}

bool UQuestCreationComponent::TryApplyNextQuestAction(UQuest* Quest, UQuestAction* ActionCandidate,
	TMap<uint32, bool>& SimulatedConditionResolutions) const
{
	if (!ActionCandidate->SimulateIsAvailable(this, SimulatedConditionResolutions))
	{
		return false;
	}
		
	Quest->AddQuestAction(ActionCandidate);
	for (const UQuestCondition* Condition : ActionCandidate->GetPostConditions())
	{
		const uint32 Id = Condition->GetId();
		SimulatedConditionResolutions.Add(Id, !Condition->bInvertCondition);
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

