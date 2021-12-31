// Copyright 2022 Jan Plähn. All Rights Reserved.

#include "QuestProviderComponent.h"
#include "AesirProceduralQuestBPLibrary.h"
#include "QuestCreator.h"
#include "QuestDataCreationComponent.h"
#include "GameFramework/GameModeBase.h"

UQuestProviderComponent::UQuestProviderComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UQuestProviderComponent::TryGetQuestData(FQuestData& OutQuestData) const
{
	const UWorld* World = GetWorld();
	if (!ensureMsgf(IsValid(World), TEXT("QuestProviderComponent: Could not find valid world")))
	{
		return false;
	}
	
	AGameModeBase* GameMode =  World->GetAuthGameMode();
	if (!ensureMsgf(IsValid(GameMode), TEXT("QuestProviderComponent: Could not find valid gamemode")))
	{
		return false;
	}

	if (!ensureMsgf(GameMode->Implements<UQuestCreator>(), TEXT("Could not find IQuestCreator Interface on gamemode")))
	{
		return false;
	}	
	UQuestDataCreationComponent* QuestDataCreator =  IQuestCreator::Execute_GetQuestDataCreationComponent(GameMode);
	if (!ensureMsgf(IsValid(QuestDataCreator), TEXT("QuestProviderComponent: Could not find quest data creator on gamemode")))
	{
		return false;
	}	
	OutQuestData = QuestDataCreator->CreateQuestData(Quest.Get());
	return true;
}

UQuestProviderPreferences* UQuestProviderComponent::GetPreferences() const
{
	return Preferences;
}

void UQuestProviderComponent::SetQuest(UQuest* NewQuest)
{
	Quest = NewQuest;
}

TSoftObjectPtr<UQuest> UQuestProviderComponent::GetQuest() const
{
	return Quest;
}

bool UQuestProviderComponent::RequestAsyncQuestGeneration()
{
	const UWorld* World = GetWorld();
	if (!ensureMsgf(IsValid(World), TEXT("QuestProviderComponent: Could not find valid world")))
	{
		return false;
	}
	
	AGameModeBase* GameMode =  World->GetAuthGameMode();
	if (!ensureMsgf(IsValid(GameMode), TEXT("QuestProviderComponent: Could not find valid gamemode")))
	{
		return false;
	}

	if (!ensureMsgf(GameMode->Implements<UQuestCreator>(), TEXT("Could not find IQuestCreator Interface on gamemode")))
	{
		return false;
	}	
	UQuestCreationComponent* QuestCreator =  IQuestCreator::Execute_GetQuestCreationComponent(GameMode);
	if (!ensureMsgf(IsValid(QuestCreator), TEXT("QuestProviderComponent: Could not find quest creator on gamemode")))
	{
		return false;
	}	
	QuestCreator->RequestQuestGeneration(this);	
	return true;
}

bool UQuestProviderComponent::PauseAsyncQuestGeneration()
{
	const UWorld* World = GetWorld();
	if (!ensureMsgf(IsValid(World), TEXT("QuestProviderComponent: Could not find valid world")))
	{
		return false;
	}
	
	AGameModeBase* GameMode =  World->GetAuthGameMode();
	if (!ensureMsgf(IsValid(GameMode), TEXT("QuestProviderComponent: Could not find valid gamemode")))
	{
		return false;
	}

	if (!ensureMsgf(GameMode->Implements<UQuestCreator>(), TEXT("Could not find IQuestCreator Interface on gamemode")))
	{
		return false;
	}	
	UQuestCreationComponent* QuestCreator =  IQuestCreator::Execute_GetQuestCreationComponent(GameMode);
	if (!ensureMsgf(IsValid(QuestCreator), TEXT("QuestProviderComponent: Could not find quest creator on gamemode")))
	{
		return false;
	}	
	QuestCreator->PauseQuestGeneration(this);		
	UAesirProceduralQuestBPLibrary::DebugLogQuest(Quest.Get());

	return true;
}
