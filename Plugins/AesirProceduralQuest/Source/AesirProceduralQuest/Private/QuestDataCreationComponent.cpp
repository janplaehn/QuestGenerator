// Fill out your copyright notice in the Description page of Project Settings.

#include "QuestDataCreationComponent.h"

UQuestDataCreationComponent::UQuestDataCreationComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UQuestDataCreationComponent::InjectDependencies(TSoftObjectPtr<UQuestCreationComponent> InQuestCreator)
{
	QuestCreator = InQuestCreator;
}

FQuestData UQuestDataCreationComponent::CreateQuestData(UQuestProviderPreferences* Preferences)
{
	FQuestData Data;
	Data.Quest = QuestCreator->CreateQuest(Preferences);
	return Data;
}

