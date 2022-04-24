// Fill out your copyright notice in the Description page of Project Settings.

#include "QuestParameter_RandomFromTable.h"
#include "QuestAction.h"

FName UQuestParameter_RandomFromTable::GenerateValue()
{	
	const TArray<FName> RowNames = DataTable->GetRowNames();
	const uint32 RandomIndex = FMath::RandRange(0, RowNames.Num()-1);
	return RowNames[RandomIndex];
}
