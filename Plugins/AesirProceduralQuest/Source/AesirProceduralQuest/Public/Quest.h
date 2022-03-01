// Copyright 2022 Jan Plähn. All Rights Reserved.

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

    virtual void AddQuestAction(const UQuestAction* NewAction);

	const TArray<const UQuestAction*>& GetActions() const;

	virtual TArray<UQuestCondition*> GetPostConditions() const override;

	float GetFitnessByTags();

	float GetFitnessByConditions(const UObject* Context);

	float GetFitnessByIntentionality();

	float GetFitnessByAffinity();

	void SetProviderData(UQuestProviderPreferences* Data);

	UQuestProviderPreferences* GetProviderData() const;

	float GetFitnessByDuplicates();

	float DebugFitness = 0;

protected:
	UPROPERTY(Transient)
	TArray<const UQuestAction*> Actions;

	UPROPERTY(Transient)
	UQuestProviderPreferences* ProviderData;

private:
	float CachedFitnessByTags = -1;
	float CachedFitnessByConditions = -1;
	float CachedFitnessByIntentionality = -1;
	float CachedFitnessByAffinity = -1;
	float CachedFitnessByDuplicates = -1;
};
