// Copyright 2022 Jan Plähn. All Rights Reserved.

#include "QuestProviderComponent.h"
#include "AesirProceduralQuestBPLibrary.h"
#include "OpenAICallGPT.h"
#include "QuestCreator.h"
#include "GameFramework/GameModeBase.h"
#include "OpenAIUtils.h"
#include "QuestCreationComponent.h"

UQuestProviderComponent::UQuestProviderComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

UQuestProviderPreferences* UQuestProviderComponent::GetPreferences() const
{
	return Preferences;
}

UQuest* UQuestProviderComponent::GetQuest() const
{
	return Quest.Get();
}

bool UQuestProviderComponent::RequestAsyncQuestGeneration() //Todo: Provide functions that just get the IQuestCreator Interface!
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
	QuestGenerationId = QuestCreator->RequestQuestGeneration(this, GetPreferences());	
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
	Quest = QuestCreator->FinishQuestGeneration(QuestGenerationId);		
	UAesirProceduralQuestBPLibrary::DebugLogQuest(this, Quest.Get(), Preferences);
	UOpenAIUtils::setOpenAIApiKey("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");
	UE_LOG(LogTemp, Verbose, TEXT("Sending OpenAI Prompt %s"), *UAesirProceduralQuestBPLibrary::CreateOpenAiPrompt(Quest.Get()));
	return true;
}
