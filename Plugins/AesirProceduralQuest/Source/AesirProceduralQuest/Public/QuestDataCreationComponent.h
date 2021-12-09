// Copyright 2022 Jan Plähn. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "QuestCreationComponent.h"
#include "QuestData.h"
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
