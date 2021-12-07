// Copyright 2021 Aesir Interactive, GmbH. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "QuestActionRow.generated.h"

class UQuestAction;

/**
 * Table row definition for a BuildingData data table.
 */
USTRUCT(BlueprintType)
struct AESIRPROCEDURALQUEST_API FQuestActionRow : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UQuestAction> QuestAction;
};
