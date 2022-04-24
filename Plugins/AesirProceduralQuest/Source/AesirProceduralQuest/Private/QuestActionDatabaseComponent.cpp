// Fill out your copyright notice in the Description page of Project Settings.

#include "QuestActionDatabaseComponent.h"

#include "QuestAction.h"
#include "QuestActionRow.h"
#include "QuestCondition.h"

UQuestActionDatabaseComponent::UQuestActionDatabaseComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UQuestActionDatabaseComponent::BeginPlay()
{
	Super::BeginPlay();
	InitPossibleQuestActions();
}

void UQuestActionDatabaseComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	for (auto It = ActionInstances.CreateConstIterator(); It; ++It)
	{
		if (IsValid(It.Value()))
		{
			It.Value()->RemoveFromRoot();
			It.Value()->ConditionalBeginDestroy();
			ActionInstances.Remove(It.Key());
		}
	}
	
	Super::EndPlay(EndPlayReason);
}

#if WITH_EDITOR
EDataValidationResult UQuestActionDatabaseComponent::IsDataValid(TArray<FText>& ValidationErrors)
{
	if (!IsValid(QuestActionDataTable) || !QuestActionDataTable->GetRowNames().Num())
		ValidationErrors.Add(FText::FromString("No valid ques action datatable assigned!"));

	return ValidationErrors.Num() ? EDataValidationResult::Invalid : EDataValidationResult::Valid;
}
#endif

void UQuestActionDatabaseComponent::InitPossibleQuestActions()
{
	QuestActionDataTable->ForeachRow<FQuestActionRow>("UQuestCreationComponent::InitPossibleQuestActions",
		[this](const FName& RowName, const FQuestActionRow& Row)
		{
			Archetypes.Add(Row.QuestAction);
		}
	);
}

UQuestAction* UQuestActionDatabaseComponent::GetRandomAction()
{	
	const int RandomIndex = FMath::RandRange(0, Archetypes.Num() - 1);

	//Todo: don't create an object, calculate an id first (how in the world??)
	uint32 NewId;
	TMap<FName, FName> NewParameterValues;
	
	Archetypes[RandomIndex]->MakeRandomParameters(NewId, NewParameterValues);
	
	if (UQuestAction** ExistingAction = ActionInstances.Find(NewId))
	{
		return *ExistingAction;
	}

	//Clean up if we have too many instances
	// if (ActionInstances.Num() > MaxNumInstancesToHold)
	// {
	// 	uint32 RemovedInstances = 0;
	// 	for (auto It = ActionInstances.CreateConstIterator(); It; ++It)
	// 	{
	// 		if (!IsValid(It.Value()))
	// 		{
	// 			ActionInstances.Remove(It.Key());
	// 			RemovedInstances++;
	// 		}
	// 		else if (!It.Value()->OwnerCount)
	// 		{
	// 			It.Value()->RemoveFromRoot();
	// 			It.Value()->ConditionalBeginDestroy();
	// 			ActionInstances.Remove(It.Key());
	// 			RemovedInstances++;
	// 		}
	//
	// 		if (RemovedInstances > 10)
	// 		{
	// 			break;
	// 		}
	// 	}
	// 	GEditor->ForceGarbageCollection(true);
	// }

	UQuestAction* NewAction = DuplicateObject(Archetypes[RandomIndex], this);
	NewAction->InitializeAsInstance(NewId, NewParameterValues);
	NewAction->AddToRoot();
	
	ActionInstances.Add(NewId, NewAction);
	return NewAction;
}

