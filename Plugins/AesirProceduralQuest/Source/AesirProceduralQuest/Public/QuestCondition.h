// Copyright 2022 Jan Plähn. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "QuestParameter.h"
#include "UObject/Object.h"
#include "QuestCondition.generated.h"

UENUM()
enum EConditionType
{
	Boolean	UMETA(DisplayName = "Boolean"),
	State	UMETA(DisplayName = "State")
};

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
	uint32 GetStateId() const;

	bool SimulateIsAvailable(const uint32 Resolution) const;

	UFUNCTION(BlueprintCallable, BlueprintPure, BlueprintNativeEvent)
	FString GetPropertyInfo() const;

	UFUNCTION(BlueprintCallable, BlueprintPure, BlueprintNativeEvent)
	EConditionType GetConditionType() const;

protected:
	virtual uint32 GenerateId() const;
	virtual uint32 GenerateStateId() const;

private:
	UPROPERTY(VisibleAnywhere)
	uint32 ConditionId;

	UPROPERTY(VisibleAnywhere)
	uint32 StateId;
};
