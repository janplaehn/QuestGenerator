// Copyright 2022 Jan Plähn. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "QuestCondition.h"
#include "IsAliveCondition.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class QUESTGENERATOR_API UIsAliveCondition : public UQuestCondition
{
	GENERATED_BODY()
public:
	virtual bool IsResolved_Implementation(const UObject* WorldContextObject) const override;

	virtual FString GetPropertyInfo_Implementation() const override;

	virtual uint32 GenerateId() const override;

	virtual void InjectParameters(const TArray<UQuestParameter*>& Parameters) override;

	virtual TSet<FName> GetParameters() const override;
	
protected:
	UPROPERTY(EditAnywhere)
	FName  CharacterName;
};
