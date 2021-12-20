// Copyright 2022 Jan Plähn. All Rights Reserved.

#include "QuestCreationComponent.h"

#include "AesirProceduralQuest.h"
#include "Quest.h"
#include "QuestActionRow.h"
#include "QuestCreator.h"
#include "Kismet/GameplayStatics.h"

UQuestCreationComponent::UQuestCreationComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UQuestCreationComponent::Initialize()
{
	InitPossibleQuestActions();
}

UQuest* UQuestCreationComponent::CreateQuest(UQuestProviderPreferences* Preferences)
{	
	UE_LOG(LogProceduralQuests, Verbose, TEXT("Generating quest for provider '%s'..."), *Preferences->ProviderName.ToString())

	const double GenerationStartTimestamp = FPlatformTime::Seconds();
	
	if (CachedPossibleQuestActions.Num() == 0) //Todo: Add ensure!
		return nullptr;

	TMap<uint32, bool> SimulatedConditionResolutions;
	UQuest* RandomQuest = NewObject<UQuest>(this);
	const int32 QuestActionCount = FMath::RandRange(QuestActionCountRange.GetLowerBound().GetValue(), QuestActionCountRange.GetUpperBound().GetValue());
	for (int32 QuestIndex = 0; QuestIndex < QuestActionCount; QuestIndex++)
	{
		if (!ensure(TryApplyNextQuestAction(RandomQuest, SimulatedConditionResolutions))) //Todo: Remove ensure once the rest is implemented
		{
			//Todo: remove the last action again
			//Todo: Start over from there
		}
	}

	const double GenerationTimeMilliseconds = (FPlatformTime::Seconds() - GenerationStartTimestamp) * 1000;

	UE_LOG(LogProceduralQuests, Verbose, TEXT("Quest generation took %f milliseconds"), GenerationTimeMilliseconds)
	
	return RandomQuest;
}

bool UQuestCreationComponent::TryApplyNextQuestAction(UQuest* Quest, TMap<uint32, bool>& SimulatedConditionResolutions) const
{
	for(int AttemptIndex = 0; AttemptIndex < MaxQuestSampleCount; AttemptIndex++)
	{
		UQuestAction* ActionCandidate = GetRandomQuestAction();
		if (ActionCandidate->SimulateIsAvailable(this, SimulatedConditionResolutions))
		{
			Quest->AddQuestAction(ActionCandidate);
			for (const UQuestCondition* Condition : ActionCandidate->GetPostConditions())
			{
				SimulatedConditionResolutions.FindOrAdd(Condition->GetId(), !Condition->bInvertCondition);
			}
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
			Conditions.Append(Row.QuestAction->GetPreConditions());
			for (UQuestCondition* Condition : Conditions)
			{
				Condition->Init();
			}
			CachedPossibleQuestActions.AddUnique(Row.QuestAction);
		}
	);
}

