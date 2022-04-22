// Copyright 2022 Jan Plähn. All Rights Reserved.

#include "QuestGenerationSnapshot.h"
#include "Quest.h"

FQuestGenerationSnapshot::FQuestGenerationSnapshot(UObject* InContext, UQuestProviderPreferences* InGenerationData)
{
	GenerationData = InGenerationData;
	Context = InContext;
	GlobalMaximum = NewObject<UQuest>(InContext);
	LocalMaximum = NewObject<UQuest>(InContext);
	Candidate = NewObject<UQuest>(InContext);

	GlobalMaximum->SetProviderData(InGenerationData);
	LocalMaximum->SetProviderData(InGenerationData);
	Candidate->SetProviderData(InGenerationData);
}
