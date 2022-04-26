// Copyright 2022 Jan Plähn. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UQuestParameter_PlayerLocationBased.h"
#include "UQuestParameter_Location.generated.h"

UCLASS(Abstract, Blueprintable)
class QUESTGENERATOR_API UQuestParameter_Location : public UQuestParameter_PlayerLocationBased
{
	GENERATED_BODY()
	
protected:
	virtual bool GenerateValueFromWorldState(const UQuest* Quest, FName& OutValue) const override;
};
