// Copyright 2022 Jan Plähn. All Rights Reserved.

#include "QuestCondition.h"

void UQuestCondition::PostInitProperties()
{
	Super::PostInitProperties();
	ConditionId = GenerateId();
}

void UQuestCondition::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	ConditionId = GenerateId();
}

void UQuestCondition::PostLoad()
{
	Super::PostLoad();
	ConditionId = GenerateId();
}

void UQuestCondition::InjectParameters(const TArray<UQuestParameter*>& Parameters)
{
	ConditionId = GenerateId();
}

bool UQuestCondition::IsResolved_Implementation(const UObject* WorldContextObject) const
{
	return false;
}

FString UQuestCondition::GetPropertyInfo_Implementation() const
{
	return FString::Printf(TEXT("bInvertCondition: %s; "), bInvertCondition ? TEXT("true") : TEXT("false"));
}

bool UQuestCondition::SimulateIsAvailable(const bool bWasPreviouslyResolved) const
{
	return !bMatchWithConditionHistory || bWasPreviouslyResolved != bInvertCondition;
}

uint32 UQuestCondition::GetId() const
{
	return ConditionId;
}

void UQuestCondition::InjectNameParameter(FName& InOutName, const TArray<UQuestParameter*>& Parameters)
{
	const auto FoundParameter = Parameters.FindByPredicate([InOutName](UQuestParameter* Parameter)
	{
		return InOutName == Parameter->GetParameterName();
	});

	if (FoundParameter == nullptr)
	{
		return;
	}

	InOutName = (*FoundParameter)->GetValueAsName();
}

uint32 UQuestCondition::GenerateId() const
{
	return GetTypeHash(GetClass());
}
