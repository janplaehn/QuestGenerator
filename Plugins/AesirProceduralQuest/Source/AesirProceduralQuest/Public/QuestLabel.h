// Copyright 2022 Jan Plähn. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "QuestLabelRow.generated.h"

USTRUCT(BlueprintType)
struct FQuestLabel
{
	GENERATED_BODY()

public:
	/** Row handle of the label id */
	UPROPERTY(EditDefaultsOnly, meta = (RowType = "QuestLabelRow"))
	FDataTableRowHandle Label;
};

class UQuestAction;

/**
 * Table row definition for a quest label data table.
 */
USTRUCT(BlueprintType)
struct AESIRPROCEDURALQUEST_API FQuestLabelRow : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Label;
};