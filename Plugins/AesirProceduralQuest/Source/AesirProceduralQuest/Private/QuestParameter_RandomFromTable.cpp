// Fill out your copyright notice in the Description page of Project Settings.

#include "QuestParameter_RandomFromTable.h"

#include "Quest.h"
#include "QuestAction.h"

void UQuestParameter_RandomFromTable::Initialize(const UQuestAction* Context)
{
	const bool bUseDesiredParameter = FMath::RandBool();
	const UQuest* OwningQuest = Context->GetOwningQuest();
	if (bUseDesiredParameter && IsValid(OwningQuest))
	{
		TableRowName = Context->GetOwningQuest()->GetProviderData()->GetRandomParameterFromPreferences(DataTable);
		return;
	}
	
	const TArray<FName> RowNames = DataTable->GetRowNames();
	const uint32 RandomIndex = FMath::RandRange(0, RowNames.Num()-1);
	TableRowName = RowNames[RandomIndex];
}

FName UQuestParameter_RandomFromTable::GetValueAsName()
{
	ensure(!TableRowName.IsNone());
	return TableRowName;
}
