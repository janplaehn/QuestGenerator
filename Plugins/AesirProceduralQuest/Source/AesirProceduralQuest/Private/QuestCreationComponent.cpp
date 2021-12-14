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
	//Todo: Initialize QuestActions Array from DataTable at initialization

	const double GenerationStartTimestamp = FPlatformTime::Seconds();
	
	if (!IsValid(QuestActionDataTable))
		return nullptr;

	if (QuestActionDataTable->GetRowNames().Num() <= 0)
		return nullptr;
	
	UQuest* RandomQuest = NewObject<UQuest>(this);
	for (int32 QuestIndex = 0; QuestIndex < QuestActionCount; QuestIndex++)
	{
		const UQuestAction* Action = GetRandomQuestAction();
		RandomQuest->AddQuestAction(Action);
	}

	const double GenerationTimeMilliseconds = (FPlatformTime::Seconds() - GenerationStartTimestamp) * 1000;

	UE_LOG(LogProceduralQuests, Verbose, TEXT("Quest generation took %f milliseconds"), GenerationTimeMilliseconds)
	
	return RandomQuest;
}

UQuestAction* UQuestCreationComponent::GetRandomQuestAction() const
{
	const TArray<FName> RowNames = QuestActionDataTable->GetRowNames();
	const int TotalActionCount = RowNames.Num();
	const int RandomRowIndex = FMath::RandRange(0, TotalActionCount - 1);
	const FName RandomRowName = RowNames[RandomRowIndex];
	const FQuestActionRow* QuestActionRow = QuestActionDataTable->FindRow<FQuestActionRow>(RandomRowName, TEXT("UQuestCreationComponent::CreateQuest"));
	return QuestActionRow->QuestAction;
}

