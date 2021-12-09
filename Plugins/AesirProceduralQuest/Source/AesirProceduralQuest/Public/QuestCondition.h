// Copyright 2022 Jan Plähn. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "QuestCondition.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class AESIRPROCEDURALQUEST_API UQuestCondition : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, BlueprintNativeEvent)
	bool IsResolved() const;
};
