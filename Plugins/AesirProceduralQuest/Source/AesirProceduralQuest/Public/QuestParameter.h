// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Engine/DataTable.h"
#include "QuestParameter.generated.h"

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
	virtual void Initialize() {};

	virtual FName GetValueAsName() PURE_VIRTUAL(UQuestParameter::GetValueAsName, return FName(););

	FName GetParameterName() const { return Name; };

	virtual uint32 GetInstanceCount() const PURE_VIRTUAL(UQuestParameter::GetInstanceCount, return 0;);
	
protected:
	UPROPERTY(EditAnywhere)
	FName Name;
};
