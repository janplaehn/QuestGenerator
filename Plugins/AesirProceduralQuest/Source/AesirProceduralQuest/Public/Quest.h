﻿// Copyright 2022 Jan Plähn. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "QuestAction.h"
#include "Quest.generated.h"

/**
 * 
 */
UCLASS()
class AESIRPROCEDURALQUEST_API UQuest : public UQuestAction
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	virtual bool IsAvailable(const UObject* WorldContextObject) const override;

	UFUNCTION(BlueprintCallable)
	virtual bool IsResolved(const UObject* WorldContextObject) const override;

    virtual void AddQuestAction(UQuestAction* NewAction);

	const TArray<UQuestAction*>& GetActions() const;

	virtual TArray<UQuestCondition*> GetPostConditions() const override;

	float GetFitnessByConditions(const UObject* Context);

	float GetFitnessByIntentionality();

	float GetFitnessByAffinity();

	void SetProviderData(UQuestProviderPreferences* Data);

	UQuestProviderPreferences* GetProviderData() const;

	float GetFitnessByDuplicates();

	float DebugFitness = 0;

	virtual void BeginDestroy() override;

protected:
	UPROPERTY(Transient)
	TArray<UQuestAction*> Actions;

	UPROPERTY(Transient)
	UQuestProviderPreferences* ProviderData;

private:
	float CachedFitnessByConditions = -1;
	float CachedFitnessByIntentionality = -1;
	float CachedFitnessByAffinity = -1;
};
