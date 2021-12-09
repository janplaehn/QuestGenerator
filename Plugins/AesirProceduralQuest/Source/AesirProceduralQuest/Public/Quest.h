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
	virtual bool IsAvailable() const override;

	UFUNCTION(BlueprintCallable)
	virtual bool IsResolved() const override;

    virtual void AddQuestAction(const UQuestAction* NewAction);
	
protected:
	UPROPERTY(Transient)
	TArray<const UQuestAction*> Actions;
};
