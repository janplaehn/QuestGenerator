// Copyright 2022 Jan Plähn. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "QuestParameter_FindSuitable.h"
#include "UQuestParameter_PlayerLocationBased.generated.h"

UCLASS(Abstract)
class QUESTGENERATOR_API UQuestParameter_PlayerLocationBased : public UQuestParameter_FindSuitable
{
	GENERATED_BODY()
	
protected:
	bool FindPlayerLocation(const UQuest* Quest, FName& OutLocationName) const;
};
