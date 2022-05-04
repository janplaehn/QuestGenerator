// Fill out your copyright notice in the Description page of Project Settings.

#include "BulkQuestCreator.h"

UBulkQuestCreator::UBulkQuestCreator()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UBulkQuestCreator::BeginPlay()
{
	ApplyGenerationProperties(StartGenerationProperties);
	GenerationId = QuestCreator->RequestQuestGeneration(this, AllPreferences[CurrentPreferenceIndex]);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UBulkQuestCreator::HandleOnTimerFinished, GenerationTime, false);
	Super::BeginPlay();
}

void UBulkQuestCreator::InjectDependencies(UQuestCreationComponent* InQuestCreator)
{
	QuestCreator = InQuestCreator;
}

void UBulkQuestCreator::ApplyGenerationProperties(const FGenerationProperties& NewProperties)
{
	QuestCreator->MinLocalIterations = NewProperties.MinLocalIterations;
	QuestCreator->AbandonBias = NewProperties.AbandonBias;
	QuestCreator->QuestActionCountRange.SetUpperBoundValue(NewProperties.QuestActionCount);
	QuestCreator->QuestActionCountRange.SetLowerBoundValue(NewProperties.QuestActionCount);
	QuestCreator->MaxQuestSampleCount = NewProperties.MaxQuestSampleCount;
}

void UBulkQuestCreator::HandleOnTimerFinished()
{
	QuestCreator->FinishQuestGeneration(GenerationId);
	CurrentIteration++;
	if (CurrentIteration >= IterationsPerSetting)
	{
		CurrentIteration = 0;
		CurrentPreferenceIndex++;
		if (!AllPreferences.IsValidIndex(CurrentPreferenceIndex))
		{
			CurrentPreferenceIndex = 0;
			CurrentGenerationStep++;
			OnPropertyChange.Broadcast();
		}		
	}

	if (CurrentGenerationStep > GenerationSteps)
	{
		return;		
	}
	
	const FGenerationProperties& NewProperties = StartGenerationProperties.InterpolateTo(EndGenerationProperties, (float)CurrentGenerationStep / (float)GenerationSteps);
	ApplyGenerationProperties(NewProperties);
	GenerationId = QuestCreator->RequestQuestGeneration(this, AllPreferences[CurrentPreferenceIndex]);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UBulkQuestCreator::HandleOnTimerFinished, GenerationTime, false);
}
