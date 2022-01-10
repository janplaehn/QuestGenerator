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
	
protected:
	UPROPERTY(Transient)
	TArray<const UQuestAction*> Actions;
};
