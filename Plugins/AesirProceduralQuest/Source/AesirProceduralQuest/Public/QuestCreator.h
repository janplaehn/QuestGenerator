// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "QuestCreationComponent.h"
#include "UObject/Interface.h"
#include "QuestCreator.generated.h"

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
};
