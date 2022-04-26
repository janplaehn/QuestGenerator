// Copyright 2022 Jan Plähn. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "QuestProviderPreferences.h"
#include "QuestAction.generated.h"

class UQuest;
class UQuestParameter;
class UQuestCondition;

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class AESIRPROCEDURALQUEST_API UQuestAction : public UDataAsset
{
	GENERATED_BODY()
	
public:
	void MakeSuitableParameters(const UQuest* InQuest, uint32& OutId, TMap<FName, FName>& OutParameterValues, TMap<TSubclassOf<UQuestParameter>, TSet<FName>>& OutParametersByClassMap) const;

	virtual void InitializeAsInstance(const uint32 InId, const TMap<FName, FName>& ParameterValues, TMap<TSubclassOf<UQuestParameter>, TSet<FName>>& InParametersByClassMap);

	virtual void InjectParameters(const TMap<FName, FName>& ParameterValues);
	
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

	TMap<TSubclassOf<UQuestParameter>, TSet<FName>> ParametersByClass;

	uint32 GetId() const;

	uint32 OwnerCount = 0;
	
protected:
	FText MakeFormattedHumanReadableName(const TMap<FName, FName>& ParameterValues) const;
	
	UPROPERTY(EditDefaultsOnly, Instanced)
	TArray<UQuestCondition*> PreConditions;

	UPROPERTY(EditDefaultsOnly, Instanced)
	TArray<UQuestCondition*> PostConditions;

private:
	UPROPERTY(EditDefaultsOnly)
	FText ReadableDescription;
	
	UPROPERTY(EditAnywhere)
	TMap<FName, TSubclassOf<UQuestParameter>> ParameterMap;

	uint32 Id = 0;
};
 