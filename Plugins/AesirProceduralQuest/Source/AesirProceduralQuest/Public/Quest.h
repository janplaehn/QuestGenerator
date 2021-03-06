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

	virtual void ClearQuest();

	virtual bool CopyFrom(const UQuest* OtherQuest);

    virtual void AddQuestAction(TWeakObjectPtr<UQuestAction> NewAction);

	bool IsEmpty() const;

	UFUNCTION(BlueprintPure)
	TArray<UQuestAction*> BlueprintGetActions() const;

	TArray<TWeakObjectPtr<UQuestAction>> GetActions() const;

	virtual TArray<UQuestCondition*> GetPostConditions() const override;

	void SetProviderData(UQuestProviderPreferences* Data);

	UFUNCTION(BlueprintPure)
	UQuestProviderPreferences* GetProviderData() const;

protected:
	TArray<TWeakObjectPtr<UQuestAction>> Actions;

	UPROPERTY(Transient)
	UQuestProviderPreferences* ProviderData;
};
