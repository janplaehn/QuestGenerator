// Copyright 2022 Jan Plähn. All Rights Reserved.

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

