// Copyright 2022 Jan Plähn. All Rights Reserved.

#include "QuestDataCreationComponent.h"

UQuestDataCreationComponent::UQuestDataCreationComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

FQuestData UQuestDataCreationComponent::CreateQuestData(UQuest* Quest)
{
	FQuestData Data;
	Data.Quest = Quest;
	return Data;
}

