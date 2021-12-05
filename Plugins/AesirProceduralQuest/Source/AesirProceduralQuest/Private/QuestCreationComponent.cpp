// Fill out your copyright notice in the Description page of Project Settings.

#include "QuestCreationComponent.h"

UQuestCreationComponent::UQuestCreationComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

UQuest* UQuestCreationComponent::CreateQuest(UQuestProviderPreferences* Preferences)
{
	//Todo: (For now) generate a random sequence of actions from the QuestActions DataTable
	return nullptr;
}

