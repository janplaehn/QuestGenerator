// Fill out your copyright notice in the Description page of Project Settings.

#include "QuestCreationComponent.h"

#include "Quest.h"
#include "QuestActionRow.h"

UQuestCreationComponent::UQuestCreationComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

UQuest* UQuestCreationComponent::CreateQuest(UQuestProviderPreferences* Preferences)
{
	//Todo: Initialize QuestActions Array from DataTable at initialization
	
	if (!IsValid(QuestActionDataTable))
		return nullptr;

	if (QuestActionDataTable->GetRowNames().Num() <= 0)
		return nullptr;
	
	UQuest* RandomQuest = NewObject<UQuest>(this);
	for (int32 QuestIndex = 0; QuestIndex < QuestActionCount; QuestIndex++)
	{
		const TSoftObjectPtr<UQuestAction> Action = GetRandomQuestAction();
		RandomQuest->AddQuestAction(Action);
	}
	return RandomQuest;
}

TSoftObjectPtr<UQuestAction> UQuestCreationComponent::GetRandomQuestAction() const
{
	const TArray<FName> RowNames = QuestActionDataTable->GetRowNames();
	const int TotalActionCount = RowNames.Num();
	const int RandomRowIndex = FMath::RandRange(0, TotalActionCount - 1);
	const FName RandomRowName = RowNames[RandomRowIndex];
	const FQuestActionRow* QuestActionRow = QuestActionDataTable->FindRow<FQuestActionRow>(RandomRowName, TEXT("UQuestCreationComponent::CreateQuest"));
	return QuestActionRow->QuestAction;
}

