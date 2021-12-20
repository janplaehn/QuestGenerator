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
	virtual void Init();
	
	//Todo: Instead of the WorldContextObject, provide the QuestController and the QuestProvider!
	UFUNCTION(BlueprintCallable, BlueprintPure, BlueprintNativeEvent)
	bool IsResolved(const UObject* WorldContextObject) const;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	bool bInvertCondition = false;

	UPROPERTY(EditAnywhere)
	bool bMatchWithConditionHistory = true;

	uint32 GetId() const;

	bool SimulateIsAvailable(const bool bWasPreviouslyResolved) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, BlueprintNativeEvent)
	FString GetPropertyInfo() const;

protected:
	virtual uint32 GenerateId() const;

private:
	uint32 ConditionId;
};
