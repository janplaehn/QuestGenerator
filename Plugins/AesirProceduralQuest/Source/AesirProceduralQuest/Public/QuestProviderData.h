// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "QuestProviderData.generated.h"

class UQuestAction;
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class AESIRPROCEDURALQUEST_API UQuestProviderData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	FName ProviderName = "GenericQuestProvider";

	UPROPERTY(EditAnywhere)
	int QuestDepth = 5;

	UPROPERTY(EditAnywhere)
	TMap<UQuestAction*, int> ActionPreferences;
};
