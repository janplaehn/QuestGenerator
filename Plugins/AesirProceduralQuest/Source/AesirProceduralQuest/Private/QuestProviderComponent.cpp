// Fill out your copyright notice in the Description page of Project Settings.

#include "QuestProviderComponent.h"
#include "QuestDataCreationComponent.h"
#include "GameFramework/GameModeBase.h"

UQuestProviderComponent::UQuestProviderComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

FQuestData UQuestProviderComponent::GetQuestData() const
{
	//Todo: Add ensures and stuff here!
	AGameModeBase* GameMode =  GetWorld()->GetAuthGameMode();
	UQuestDataCreationComponent* QuestDataCreator = Cast<UQuestDataCreationComponent>(GameMode->GetComponentByClass(UQuestDataCreationComponent::StaticClass()));
	return QuestDataCreator->CreateQuestData(Preferences);
}
