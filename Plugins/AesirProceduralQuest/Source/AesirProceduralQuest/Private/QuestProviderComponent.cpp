// Copyright 2022 Jan Plähn. All Rights Reserved.

#include "QuestProviderComponent.h"

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

	OutQuestData =  QuestDataCreator->CreateQuestData(Preferences);
	return true;
}
