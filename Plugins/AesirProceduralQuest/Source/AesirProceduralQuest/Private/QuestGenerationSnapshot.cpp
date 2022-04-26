// Copyright 2022 Jan Plähn. All Rights Reserved.

#include "QuestGenerationSnapshot.h"
#include "Quest.h"

FQuestGenerationSnapshot::FQuestGenerationSnapshot(UObject* InContext, UQuestProviderPreferences* InGenerationData, const uint32 InActionCount)
{
	GenerationData = InGenerationData;
	GenerationData->FitnessWeights.Normalize();
	Context = InContext;
	ActionCount = InActionCount;
	GlobalMaximum = NewObject<UQuest>(InContext);
	LocalMaximum = NewObject<UQuest>(InContext);
	Candidate = NewObject<UQuest>(InContext);

	GlobalMaximum->SetProviderData(InGenerationData);
	LocalMaximum->SetProviderData(InGenerationData);
	Candidate->SetProviderData(InGenerationData);
}
