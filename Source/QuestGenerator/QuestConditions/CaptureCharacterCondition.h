// Copyright 2022 Jan Plähn. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "QuestCondition.h"
#include "CaptureCharacterCondition.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class QUESTGENERATOR_API UCaptureCharacterCondition : public UQuestCondition
{
	GENERATED_BODY()
public:
	virtual bool IsResolved_Implementation(const UObject* WorldContextObject) const override;

	virtual FString GetPropertyInfo_Implementation() const override;

	virtual uint32 GenerateId() const override;

	virtual void InjectParameters(const TMap<FName, FName>& ParameterValues) override;
	
protected:
	UPROPERTY(EditAnywhere)
	FName  CharacterName;
};
