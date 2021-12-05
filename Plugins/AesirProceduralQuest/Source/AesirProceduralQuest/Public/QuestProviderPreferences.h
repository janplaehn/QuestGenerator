// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestCondition.h"
#include "Engine/DataAsset.h"
#include "QuestProviderPreferences.generated.h"

class UQuestAction;
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class AESIRPROCEDURALQUEST_API UQuestProviderPreferences : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	FName ProviderName = "GenericQuestProvider";

	UPROPERTY(EditAnywhere)
	TArray<FName> AssociatedLabels;

	UPROPERTY(EditAnywhere)
	TArray<UQuestCondition*> DesiredConditions;
};
