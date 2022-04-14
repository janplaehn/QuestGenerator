// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestParameter.h"
#include "QuestParameter_RandomFromTable.generated.h"

/**
 * 
 */
UCLASS()
class AESIRPROCEDURALQUEST_API UQuestParameter_RandomFromTable : public UQuestParameter
{
	GENERATED_BODY()
public:
	virtual void Initialize(const UQuestAction* Context) override;

	virtual FName GetValueAsName() override;
	
protected:
	UPROPERTY(EditAnywhere)
	UDataTable* DataTable;

	FName TableRowName = FName();
};
