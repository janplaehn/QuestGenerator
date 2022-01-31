﻿// Copyright 2022 Jan Plähn. All Rights Reserved.

#include "QuestAction.h"

#include "AesirProceduralQuestBPLibrary.h"

UQuestAction* UQuestAction::MakeRandomInstance(UObject* Outer) const
{
	UQuestAction* DuplicateAction = DuplicateObject(this, Outer);
	DuplicateAction->InitializeAsInstance();
	return DuplicateAction;
}

UQuestAction* UQuestAction::DuplicateInstance(UObject* Outer) const
{
	UQuestAction* DuplicateAction = DuplicateObject(this, Outer);
	return DuplicateAction;
}

void UQuestAction::InitializeAsInstance()
{	
	for (UQuestParameter* Parameter : Parameters)
	{
		Parameter->Initialize();
	}
	InjectParameters();
}

void UQuestAction::InjectParameters()
{
	UAesirProceduralQuestBPLibrary::InjectNameParameter(CharacterImpact.Character, Parameters);
	for (UQuestCondition* Condition : PreConditions)
	{
		Condition->InjectParameters(Parameters);
	}
	for (UQuestCondition* Condition : PostConditions)
	{
		Condition->InjectParameters(Parameters);
	}
	ReadableDescription = MakeFormattedHumanReadableName();
}

uint32 UQuestAction::GetPossibleInstanceCount() const
{
	uint32 OutInstanceCount = 1;
	for (UQuestParameter* Parameter : Parameters)
	{
		OutInstanceCount *= Parameter->GetInstanceCount();
	}
	return OutInstanceCount;
}

bool UQuestAction::IsAvailable(const UObject* WorldContextObject) const
{
	for (UQuestCondition* Condition : PreConditions)
	{
		if (!Condition->IsResolved(WorldContextObject))
		{
			return false;
		}
	}
	return true;
}

bool UQuestAction::SimulateIsAvailable(const UObject* WorldContextObject, TMap<uint32, bool> SimulatedConditionResolutions) const
{
	for (UQuestCondition* Condition : PreConditions)
	{

		if (Condition->GetName().StartsWith("HasItem") && Condition->bInvertCondition)
		{
			int x = 0;
		}
		
		const uint32 Id = Condition->GetId();
		bool* FoundResolution = SimulatedConditionResolutions.Find(Id);

		if (FoundResolution != nullptr)
		{
			if (!Condition->SimulateIsAvailable(*FoundResolution))
			{
				return false;
			}
		}
		else
		{
			if (!Condition->IsResolved(WorldContextObject))
			{
				return false;
			}
		}
	}
	return true;
}

bool UQuestAction::IsResolved(const UObject* WorldContextObject) const
{
	for (UQuestCondition* Condition : PostConditions)
	{
		if (!Condition->IsResolved(WorldContextObject))
		{
			return false;
		}
	}
	return true;
}

const TArray<UQuestCondition*>& UQuestAction::GetPreConditions() const
{
	return PreConditions;
}

TArray<UQuestCondition*> UQuestAction::GetPostConditions() const
{
	return PostConditions;
}

FText UQuestAction::GetDescription() const
{
	return ReadableDescription;
}

FText UQuestAction::MakeFormattedHumanReadableName() const
{
	FText OutText = ReadableDescription;
	for (UQuestParameter* Parameter : Parameters)
	{
		FString ParameterName = Parameter->GetParameterName().ToString();
		FText ValueAsName =  FText::FromName(Parameter->GetValueAsName());
		OutText = FText::FormatNamed(OutText, *ParameterName, ValueAsName);
	}
	return OutText;	
}
