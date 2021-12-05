// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "QuestProviderComponent.generated.h"

class UQuestLocation;
class UQuestProviderData;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class AESIRPROCEDURALQUEST_API UQuestProviderComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UQuestProviderComponent();

protected:
	FName Name = "GenericQuestProvider";

	UPROPERTY(EditAnywhere)
	UQuestProviderData* QuestProviderData = nullptr;

	UPROPERTY(EditAnywhere)
	UQuestLocation* Location = nullptr;

	//Todo: Add possible Goals
};
