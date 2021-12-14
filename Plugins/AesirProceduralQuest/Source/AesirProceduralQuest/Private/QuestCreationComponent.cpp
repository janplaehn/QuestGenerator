// Copyright 2022 Jan Plähn. All Rights Reserved.

#include "QuestCreationComponent.h"

#include "AesirProceduralQuest.h"
#include "Quest.h"
#include "QuestActionRow.h"
#include "Kismet/GameplayStatics.h"

UQuestCreationComponent::UQuestCreationComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

UQuest* UQuestCreationComponent::CreateQuest(UQuestProviderPreferences* Preferences)
{	
	UE_LOG(LogProceduralQuests, Verbose, TEXT("Generating quest for provider '%s'..."), *Preferences->ProviderName.ToString())

	const double GenerationStartTimestamp = FPlatformTime::Seconds();
	
	if (!IsValid(QuestActionDataTable))
		return nullptr;

	if (QuestActionDataTable->GetRowNames().Num() <= 0)
		return nullptr;

	if (CachedPossibleQuestActions.Num() <= 0)
		CachePossibleQuestActions();

	TArray<UQuestCondition*> AccumulatedPostConditions;
	UQuest* RandomQuest = NewObject<UQuest>(this);
	const int32 QuestActionCount = FMath::RandRange(QuestActionCountRange.GetLowerBound().GetValue(), QuestActionCountRange.GetUpperBound().GetValue());
	for (int32 QuestIndex = 0; QuestIndex < QuestActionCount; QuestIndex++)
	{
		if (!ensure(TryApplyNextQuestAction(RandomQuest, AccumulatedPostConditions))) //Todo: Remove ensure once the rest is implemented
		{
			//Todo: remove the last action again
			//Todo: Start over from there
		}
	}

	const double GenerationTimeMilliseconds = (FPlatformTime::Seconds() - GenerationStartTimestamp) * 1000;

	UE_LOG(LogProceduralQuests, Verbose, TEXT("Quest generation took %f milliseconds"), GenerationTimeMilliseconds)
	
	return RandomQuest;
}

bool UQuestCreationComponent::TryApplyNextQuestAction(UQuest* Quest, TArray<UQuestCondition*>& AccumulatedPostConditions) const
{
	const int MaxSampleCount = 20; //Todo: Might need to reduce this, set it via property or set it dynamically
	for(int AttemptIndex = 0; AttemptIndex < MaxSampleCount; AttemptIndex++)
	{
		UQuestAction* ActionCandidate = GetRandomQuestAction();
		if (ActionCandidate->SimulateIsAvailable(this, AccumulatedPostConditions))
		{
			Quest->AddQuestAction(ActionCandidate);
			AccumulatedPostConditions.Append(ActionCandidate->GetPostConditions());
			return true;
		}
		else
		{
			UE_LOG(LogProceduralQuests, Verbose, TEXT("Could not append action %s at Index %d"), *ActionCandidate->GetName(), Quest->GetActions().Num())
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

void UQuestCreationComponent::CachePossibleQuestActions()
{
	UE_LOG(LogProceduralQuests, Verbose, TEXT("Caching %d possible quest actions"), QuestActionDataTable->GetRowNames().Num());
	
	QuestActionDataTable->ForeachRow<FQuestActionRow>("UQuestCreationComponent::CachePossibleQuestActions",
		[this](const FName& RowName, const FQuestActionRow& Row)
		{
			CachedPossibleQuestActions.AddUnique(Row.QuestAction);
		}
	);
}

