// Copyright 2022 Jan Plähn. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "QuestCondition.h"
#include "FindCharacterCondition.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class QUESTGENERATOR_API UFindCharacterCondition : public UQuestCondition
{
	GENERATED_BODY()
public:
	virtual bool IsResolved_Implementation(const UObject* WorldContextObject) const override;
	
	virtual bool SimulateIsResolved_Implementation(const UObject* WorldContextObject, TArray<UQuestCondition*>& SimulatedPostConditions) const override;

	virtual FString GetPropertyInfo_Implementation() const override;
	
protected:
	UPROPERTY(EditAnywhere)
	FName  CharacterName;
};
