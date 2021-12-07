// Fill out your copyright notice in the Description page of Project Settings.

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

    virtual void AddQuestAction(const TSoftObjectPtr<UQuestAction> NewAction);
	
protected:
	UPROPERTY(Transient)
	TArray<TSoftObjectPtr<UQuestAction>> Actions;
};
