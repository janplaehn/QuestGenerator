// Copyright 2022 Jan Plähn. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "QuestCondition.h"
#include "QuestLabel.h"
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
	FQuestLabelCollection AssociatedLabels;

	UPROPERTY(EditAnywhere)
	TArray<TSoftClassPtr<UQuestCondition>> DesiredConditions;
};
