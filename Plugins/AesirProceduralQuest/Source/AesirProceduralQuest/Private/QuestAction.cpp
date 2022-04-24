// Copyright 2022 Jan Plähn. All Rights Reserved.

#include "QuestAction.h"
#include "AesirProceduralQuestBPLibrary.h"
#include "Quest.h"


void UQuestAction::MakeRandomParameters(uint32& OutId, TMap<FName, FName>& OutParameterValues) const
{
	OutId = GetTypeHash(GetClass());
	
	for (UQuestParameter* Param : Parameters)
	{
		const FName& Value = Param->GenerateValue();
		OutParameterValues.Add(Param->Name, Value);
		OutId = HashCombine(OutId, GetTypeHash(Value));
	}
}

void UQuestAction::InitializeAsInstance(const uint32 InId, const TMap<FName, FName>& ParameterValues)
{
	Id = InId;
	InjectParameters(ParameterValues);
}

void UQuestAction::InjectParameters(const TMap<FName, FName>& ParameterValues)
{
	UAesirProceduralQuestBPLibrary::InjectNameParameter(CharacterImpact.Character, ParameterValues);
	for (UQuestCondition* Condition : PreConditions)
	{
		Condition->InjectParameters(ParameterValues);
	}
	for (UQuestCondition* Condition : PostConditions)
	{
		Condition->InjectParameters(ParameterValues);
	}
	ReadableDescription = MakeFormattedHumanReadableName(ParameterValues);
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

bool UQuestAction::SimulateIsAvailable(const UObject* WorldContextObject, TMap<uint32, uint32> SimulatedConditionResolutions) const
{
	for (UQuestCondition* Condition : PreConditions)
	{

		if (Condition->GetName().StartsWith("HasItem") && Condition->bInvertCondition)
		{
			int x = 0;
		}
		
		const uint32 ConditionId = Condition->GetId();
		uint32* FoundResolution = SimulatedConditionResolutions.Find(ConditionId);

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

UQuest* UQuestAction::GetOwningQuest() const
{
	return Cast<UQuest>(GetOuter());
}

uint32 UQuestAction::GetId() const
{
	return Id;
}

FText UQuestAction::MakeFormattedHumanReadableName(const TMap<FName, FName>& ParameterValues) const
{
	FText OutText = ReadableDescription;
	for (const auto Kvp : ParameterValues)
	{
		OutText = FText::FormatNamed(OutText, *Kvp.Key.ToString(), FText::FromName(Kvp.Value));
	}
	return OutText;	
}
