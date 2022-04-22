// Copyright 2022 Jan Plähn. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "QuestAction.h"
#include "Quest.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class AESIRPROCEDURALQUEST_API UQuest : public UQuestAction
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
	virtual bool IsAvailable(const UObject* WorldContextObject) const override;

	UFUNCTION(BlueprintCallable)
	virtual bool IsResolved(const UObject* WorldContextObject) const override;

	virtual bool CopyFrom(const UQuest* OtherQuest);

    virtual void AddQuestAction(UQuestAction* NewAction);

	virtual void Clear();

	bool IsEmpty() const;

	UFUNCTION(BlueprintPure)
	const TArray<UQuestAction*>& GetActions() const;

	virtual TArray<UQuestCondition*> GetPostConditions() const override;

	float GetFitnessByConditions(const UObject* Context);

	float GetFitnessByIntentionality();

	float GetFitnessByAffinity();

	void SetProviderData(UQuestProviderPreferences* Data);

	UFUNCTION(BlueprintPure)
	UQuestProviderPreferences* GetProviderData() const;

protected:
	UPROPERTY(Transient)
	TArray<UQuestAction*> Actions;

	UPROPERTY(Transient)
	UQuestProviderPreferences* ProviderData;

private:
	//Todo: Don't store this in the quest but move it to the generation-snapshot
	float CachedFitnessByConditions = -1;
	float CachedFitnessByIntentionality = -1;
	float CachedFitnessByAffinity = -1;
};
