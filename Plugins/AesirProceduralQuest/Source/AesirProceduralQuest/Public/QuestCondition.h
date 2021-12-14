// Copyright 2022 Jan Plähn. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "QuestCondition.generated.h"

/**
 * 
 */
UCLASS(EditInlineNew, Blueprintable, Abstract)
class AESIRPROCEDURALQUEST_API UQuestCondition : public UObject
{
	GENERATED_BODY()

public:
	//Todo: Instead of the WorldContextObject, provide the QuestController and the QuestProvider!
	UFUNCTION(BlueprintCallable, BlueprintPure, BlueprintNativeEvent)
	bool IsResolved(const UObject* WorldContextObject) const;

	//Todo: Instead of the WorldContextObject, provide the QuestController and the QuestProvider!
	UFUNCTION(BlueprintCallable, BlueprintPure, BlueprintNativeEvent)
	bool SimulateIsResolved(const UObject* WorldContextObject, TArray<UQuestCondition*>& SimulatedPostConditions) const;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	bool bInvertCondition = false;

	UFUNCTION(BlueprintCallable, BlueprintPure, BlueprintNativeEvent)
	FString GetPropertyInfo() const;
};
