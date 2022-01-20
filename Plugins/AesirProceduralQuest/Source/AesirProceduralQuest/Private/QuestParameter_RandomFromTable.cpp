// Fill out your copyright notice in the Description page of Project Settings.

#include "QuestParameter_RandomFromTable.h"

void UQuestParameter_RandomFromTable::Initialize()
{
	const TArray<FName> RowNames = DataTable->GetRowNames();
	const uint32 RandomIndex = FMath::RandRange(0, RowNames.Num()-1);
	TableRowName = RowNames[RandomIndex];
}

FName UQuestParameter_RandomFromTable::GetValueAsName()
{
	ensure(!TableRowName.IsNone());
	return TableRowName;
}
