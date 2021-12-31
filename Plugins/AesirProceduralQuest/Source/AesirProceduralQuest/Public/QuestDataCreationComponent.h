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

	FQuestData CreateQuestData(UQuest* Quest);

protected:
	TSoftObjectPtr<UQuestCreationComponent> QuestCreator;
};
