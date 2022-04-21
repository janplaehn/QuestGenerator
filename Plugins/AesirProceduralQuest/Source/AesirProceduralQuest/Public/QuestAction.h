// Copyright 2022 Jan Plähn. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "QuestCondition.h"
#include "QuestLabel.h"
#include "QuestParameter.h"
#include "QuestProviderPreferences.h"

#include "QuestAction.generated.h"

class UQuest;
/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class AESIRPROCEDURALQUEST_API UQuestAction : public UDataAsset
{
	GENERATED_BODY()

public:
	UQuestAction* MakeRandomInstance(UObject* Outer) const;

	virtual void InitializeAsInstance();

	virtual void InjectParameters();

	uint32 GetPossibleInstanceCount() const;
	
	virtual bool IsAvailable(const UObject* WorldContextObject) const;

	virtual bool SimulateIsAvailable(const UObject* WorldContextObject, TMap<uint32, uint32> SimulatedConditionResolutions) const;

	virtual bool IsResolved(const UObject* WorldContextObject) const;

	const TArray<UQuestCondition*>& GetPreConditions() const;

	virtual TArray<UQuestCondition*> GetPostConditions() const;

	UFUNCTION(BlueprintPure)
	FText GetDescription() const;
	
	UQuest* GetOwningQuest() const;

	UPROPERTY(EditAnywhere)
	FCharacterAffinity CharacterImpact;

	uint32 GetId() const;
	
protected:
	FText MakeFormattedHumanReadableName() const;
	
	UPROPERTY(EditDefaultsOnly, Instanced)
	TArray<UQuestCondition*> PreConditions;

	UPROPERTY(EditDefaultsOnly, Instanced)
	TArray<UQuestCondition*> PostConditions;

private:
	UPROPERTY(EditDefaultsOnly)
	FText ReadableDescription;
	
	UPROPERTY(EditAnywhere, Instanced)
	TArray<UQuestParameter*> Parameters;

	TArray<FName> ParameterValues;

	void GenerateId();

	uint32 Id = 0;
};
 