// Copyright 2022 Jan Plähn. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "QuestParameter.h"
#include "UObject/Object.h"
#include "QuestCondition.generated.h"

/**
 * 
 */
UCLASS(EditInlineNew, Blueprintable, Abstract)
class AESIRPROCEDURALQUEST_API UQuestCondition : public UObject
{
	GENERATED_BODY()

public:
	virtual void PostInitProperties() override;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

	virtual void PostLoad() override;
	
	//Todo: Instead of the WorldContextObject, provide the QuestController and the QuestProvider!
	UFUNCTION(BlueprintCallable, BlueprintPure, BlueprintNativeEvent)
	bool IsResolved(const UObject* WorldContextObject) const;
	
	virtual void InjectParameters(const TArray<UQuestParameter*>& Parameters);

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	bool bInvertCondition = false;

	UPROPERTY(EditAnywhere)
	bool bMatchWithConditionHistory = true;

	uint32 GetId() const;

	bool SimulateIsAvailable(const bool bWasPreviouslyResolved) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, BlueprintNativeEvent)
	FString GetPropertyInfo() const;

protected:
	virtual uint32 GenerateId() const;

private:
	UPROPERTY(VisibleAnywhere)
	uint32 ConditionId;
};
