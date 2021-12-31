// Copyright 2022 Jan Plähn. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "QuestCreator.generated.h"

class UQuestCreationComponent;
class UQuestDataCreationComponent;

UINTERFACE(MinimalAPI)
class UQuestCreator : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class AESIRPROCEDURALQUEST_API IQuestCreator
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	UQuestDataCreationComponent* GetQuestDataCreationComponent() const;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	UQuestCreationComponent* GetQuestCreationComponent() const;
};
