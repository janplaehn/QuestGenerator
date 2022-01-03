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
	for (UQuestProviderComponent* Provider : QuestRequesters)
	{
		TSoftObjectPtr<UQuest> OldQuest = Provider->GetQuest();
		UQuest* NewQuest = CreateRandomQuest();
		UQuest* SelectedQuest = UQuestFitnessUtils::SelectFittest(OldQuest.Get(), NewQuest, Provider->GetPreferences());
		Provider->SetQuest(SelectedQuest);
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
		const UQuestAction* ActionCandidate = GetRandomQuestAction();
		if (ActionCandidate->GetName() == "QA_KillAladdin")
		{
			UE_LOG(LogTemp, Warning, TEXT("KILLALADDIN"));
		}
		if (ActionCandidate->SimulateIsAvailable(this, SimulatedConditionResolutions))
		{
			Quest->AddQuestAction(ActionCandidate);
			for (const UQuestCondition* Condition : ActionCandidate->GetPostConditions())
			{
				const uint32 Id = Condition->GetId();
				SimulatedConditionResolutions.FindOrAdd(Id, !Condition->bInvertCondition);
			}
			return true;
		}
		else
		{
			UE_LOG(LogProceduralQuests, VeryVerbose, TEXT("Could not append action %s at Index %d"), *ActionCandidate->GetName(), Quest->GetActions().Num())
		}
	}
	return false;
}

UQuestAction* UQuestCreationComponent::GetRandomQuestAction() const
{
	const int TotalActionCount = CachedPossibleQuestActions.Num();
	if (!ensureMsgf(TotalActionCount > 0, TEXT("QuestCreationComponent: Could not find cached quests.")))
	{
		return nullptr;
	}
	
	const int RandomIndex = FMath::RandRange(0, TotalActionCount - 1);
	return CachedPossibleQuestActions[RandomIndex];
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

