// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "QuestCreationComponent.h"
#include "QuestData.h"
#include "QuestProviderComponent.h"
#include "Components/ActorComponent.h"
#include "QuestDataCreationComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AESIRPROCEDURALQUEST_API UQuestDataCreationComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UQuestDataCreationComponent();

	UFUNCTION(BlueprintCallable)
	void InjectDependencies(TSoftObjectPtr<UQuestCreationComponent> InQuestCreator);

	FQuestData CreateQuestData(UQuestProviderPreferences* Preferences);

protected:
	TSoftObjectPtr<UQuestCreationComponent> QuestCreator;
};
