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
	
	const int32 QuestActionCount = IsValid(Snapshot.GlobalMaximum) && Snapshot.GlobalMaximum->GetActions().Num() ? Snapshot.GlobalMaximum->GetActions().Num() : FMath::RandRange(QuestActionCountRange.GetLowerBound().GetValue(), QuestActionCountRange.GetUpperBound().GetValue());
	UQuest* NewQuest;
	if (Snapshot.IterationsSinceLastLocalImprovement >= Snapshot.TotalIterations / 4) //Todo: Magic Value!
	{
		if (IsValid(Snapshot.LocalMaximum))
		{
			Snapshot.LocalMaximum->ConditionalBeginDestroy();
			Snapshot.LocalMaximum = nullptr;
		}
		NewQuest = CreateRandomQuest(QuestActionCount, Snapshot.GenerationData.Get());
		Snapshot.IterationsSinceLastLocalImprovement = 0;
	}
	else
	{
		NewQuest = MutateQuest(Snapshot.LocalMaximum, QuestActionCount, Snapshot.GenerationData.Get());
	}
	if (!IsValid(NewQuest))
	{
		Snapshot.NullQuestCount++;
		return;
	}
	NewQuest->SetProviderData(Snapshot.GenerationData.Get());
	UQuest* NewLocalMaximum = UQuestFitnessUtils::SelectFittest(this, Snapshot.LocalMaximum, NewQuest, Snapshot.GenerationData.Get());
	if (NewQuest != NewLocalMaximum)
	{
		NewQuest->ConditionalBeginDestroy();
		NewQuest = nullptr;
	}
	else
	{
		if (IsValid(Snapshot.LocalMaximum))
		{
			Snapshot.LocalMaximum->ConditionalBeginDestroy();
			Snapshot.LocalMaximum = nullptr;
		}
	}
	Snapshot.LocalMaximum = NewLocalMaximum;
	UQuest* NewGlobalMaximum = UQuestFitnessUtils::SelectFittest(this, Snapshot.GlobalMaximum, NewLocalMaximum, Snapshot.GenerationData.Get());
	if (NewQuest == NewGlobalMaximum)
	{
		UE_LOG(LogProceduralQuests, Verbose, TEXT("GLOBAL MAXIMUM | Current Fitness: [%f]."), UQuestFitnessUtils::CalculateWeightedFitness(this, NewGlobalMaximum, Snapshot.GenerationData.Get()));
		OnQuestUpdated.Broadcast(Snapshot);
		Snapshot.IterationsSinceLastGlobalImprovement = 0;
		if (IsValid(Snapshot.GlobalMaximum))
		{
			Snapshot.GlobalMaximum->ConditionalBeginDestroy();
			Snapshot.GlobalMaximum = nullptr;
		}
	}
	else if (NewQuest == NewLocalMaximum)
	{
		Snapshot.IterationsSinceLastLocalImprovement = 0;
	}
	if (NewGlobalMaximum)
	{
		if (IsValid(Snapshot.GlobalMaximum))
		{
			Snapshot.GlobalMaximum->ConditionalBeginDestroy();
			Snapshot.GlobalMaximum = nullptr;
		}
		Snapshot.GlobalMaximum = NewGlobalMaximum;
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
			//GenerationSnapshots.Add(Kvp.Key, Kvp.Value);
		}
	}	
}

UQuest* UQuestCreationComponent::CreateRandomQuest(const uint32 QuestActionCount, UQuestProviderPreferences* Preferences)
{	
	if (!ensureMsgf(CachedPossibleQuestActions.Num() != 0, TEXT("Quest actions have not been initialized")))
		return nullptr;

	TMap<uint32, uint32> SimulatedConditionResolutions;
	UQuest* RandomQuest = NewObject<UQuest>(this);
	RandomQuest->SetProviderData(Preferences);
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

UQuest* UQuestCreationComponent::MutateQuest(UQuest* BaseQuest, const int32 QuestActionCount, UQuestProviderPreferences* GenerationData)
{
	const int MutationIndex = FMath::RandRange(0,4);
	if (MutationIndex == 0)
	{
		return MutateQuestByScramblingActions(BaseQuest, QuestActionCount, GenerationData);
	}
	else
	{
		return MutateQuestByReplaceAction(BaseQuest,QuestActionCount, GenerationData);
	}
}

UQuest* UQuestCreationComponent::MutateQuestByReplaceAction(UQuest* BaseQuest, const int32 QuestActionCount, UQuestProviderPreferences* GenerationData)
{
	if (!IsValid(BaseQuest))
	{
		return CreateRandomQuest(QuestActionCount, GenerationData);
	}
	UQuest* NewQuest = NewObject<UQuest>(this);
	NewQuest->SetProviderData(GenerationData);
	
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

UQuest* UQuestCreationComponent::MutateQuestByScramblingActions(UQuest* BaseQuest, const int32 QuestActionCount, UQuestProviderPreferences* GenerationData)
{
	if (!IsValid(BaseQuest))
    {
		return CreateRandomQuest(QuestActionCount, GenerationData);
    }
    UQuest* NewQuest = NewObject<UQuest>(this);
	NewQuest->SetProviderData(GenerationData);
    
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

