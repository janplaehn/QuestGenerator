// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "QuestAction.generated.h"

/**
 * 
 */
UCLASS()
class AESIRPROCEDURALQUEST_API UQuestAction : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	FName Name = "GenericQuestAction";

	//Todo: Add Parameters used for this quest(?)

	//Todo: Add Preconditions that must be true

	//Todo: Add Action Effect
};
 