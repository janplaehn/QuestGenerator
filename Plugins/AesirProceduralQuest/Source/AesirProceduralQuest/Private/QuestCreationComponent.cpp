// Copyright 2022 Jan Plähn. All Rights Reserved.

#include "QuestCreationComponent.h"
#include "QuestProviderComponent.h"
#include "AesirProceduralQuest.h"
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
}

void UQuestCreationComponent::PauseQuestGeneration(UQuestProviderComponent* QuestProviderComponent)
{
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
			TSoftObjectPtr<UQuest> OldQuest = Provider->GetQuest();
			UQuest* NewQuest = CreateRandomQuest();
			const UQuest* SelectedQuest = UQuestFitnessUtils::SelectFittest(this, OldQuest.Get(), NewQuest, Provider->GetPreferences());
			if (NewQuest != SelectedQuest && IsValid(NewQuest))
			{
				NewQuest->MarkPendingKill();
			}
			else if (OldQuest.IsValid())
			{
				OldQuest->MarkPendingKill();
			}
			Provider->SetQuest(SelectedQuest);
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
		if (!TryApplyNextQuestAction(RandomQuest, SimulatedConditionResolutions))
		{
			RandomQuest->MarkPendingKill();
			return nullptr;
		}
	}
	
	return RandomQuest;
}

bool UQuestCreationComponent::TryApplyNextQuestAction(UQuest* Quest, TMap<uint32, bool>& SimulatedConditionResolutions) const
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
			const uint32 Id = Condition->GetId();
			SimulatedConditionResolutions.FindOrAdd(Id, !Condition->bInvertCondition);
		}
		return true;
	}
	return false;
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

