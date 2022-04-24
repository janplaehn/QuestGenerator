// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Engine/DataTable.h"
#include "QuestParameter.generated.h"

class UQuestAction;
/**
* Table row definition for a quest parameter data table
*/
USTRUCT(BlueprintType)
struct AESIRPROCEDURALQUEST_API FQuestParameterRow : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Value;
};

/**
 * 
 */
UCLASS(EditInlineNew, Blueprintable, Abstract)
class AESIRPROCEDURALQUEST_API UQuestParameter : public UObject
{
	GENERATED_BODY()
public:
	virtual FName GenerateValue() PURE_VIRTUAL(UQuestParameter::GetValueAsName, return FName(););
	
	UPROPERTY(EditAnywhere)
	FName Name;
};
