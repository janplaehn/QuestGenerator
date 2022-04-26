﻿// Copyright 2022 Jan Plähn. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "QuestCondition.generated.h"

class UQuestParameter;

UENUM()
enum EConditionType
{
	Boolean	UMETA(DisplayName = "Boolean"),
	State	UMETA(DisplayName = "State")
};

/**
 * 
 */
UCLASS(EditInlineNew, Blueprintable, Abstract)
class AESIRPROCEDURALQUEST_API UQuestCondition : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, BlueprintNativeEvent)
	bool IsResolved(const UObject* WorldContextObject) const;
	
	virtual void InjectParameters(const TMap<FName, FName>& ParameterValues);


	//Todo: Remove
	virtual TSet<FName> GetParameters() const {return TSet<FName>();}

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	bool bInvertCondition = false;

	UPROPERTY(EditAnywhere)
	bool bMatchWithConditionHistory = true;

	virtual uint32 GenerateId() const;
	virtual uint32 GenerateStateId() const;

	uint32 GetId() const;
	uint32 GetStateId() const;

	bool SimulateIsAvailable(const uint32 Resolution) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, BlueprintNativeEvent)
	FString GetPropertyInfo() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, BlueprintNativeEvent)
	EConditionType GetConditionType() const;

private:
	UPROPERTY(VisibleAnywhere)
	uint32 ConditionId;

	UPROPERTY(VisibleAnywhere)
	uint32 StateId;
};
