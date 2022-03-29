// Copyright 2022 Jan Plähn. All Rights Reserved.

#include "QuestProviderComponent.h"
#include "AesirProceduralQuestBPLibrary.h"
#include "OpenAICallGPT.h"
#include "QuestCreator.h"
#include "QuestDataCreationComponent.h"
#include "GameFramework/GameModeBase.h"
#include "OpenAIUtils.h"
#include "QuestFitnessBPLibrary.h"

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
	const float FitnessWeightSum = Preferences->FitnessWeights.AffinityWeight + Preferences->FitnessWeights.ConditionWeight + Preferences->FitnessWeights.IntentionalityWeight + Preferences->FitnessWeights.TagWeight+ Preferences->FitnessWeights.DuplicateWeight;
	Preferences->FitnessWeights.AffinityWeight /= FitnessWeightSum;
	Preferences->FitnessWeights.ConditionWeight /= FitnessWeightSum;
	Preferences->FitnessWeights.IntentionalityWeight /= FitnessWeightSum;
	Preferences->FitnessWeights.TagWeight /= FitnessWeightSum;
	Preferences->FitnessWeights.DuplicateWeight /= FitnessWeightSum;

	return Preferences;
}

void UQuestProviderComponent::SetQuest(UQuest* NewQuest)
{
	if (UQuestFitnessUtils::CalculateWeightedFitness(this, NewQuest, GetPreferences()) < UQuestFitnessUtils::CalculateWeightedFitness(this, Quest.Get(), GetPreferences()))
	{
		UE_LOG(LogTemp, Error, TEXT("SOMETHING IS WRONG!"));			
	}
	Quest = NewQuest;
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
	UAesirProceduralQuestBPLibrary::DebugLogQuest(this, Quest.Get(), Preferences);
	UOpenAIUtils::setOpenAIApiKey("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");
	UE_LOG(LogTemp, Verbose, TEXT("Sending OpenAI Prompt %s"), *UAesirProceduralQuestBPLibrary::CreateOpenAiPrompt(Quest.Get()));
	return true;
}
