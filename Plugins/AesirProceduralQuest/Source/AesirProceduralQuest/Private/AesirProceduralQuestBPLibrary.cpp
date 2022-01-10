// Copyright 2022 Jan Plähn. All Rights Reserved.

#include "AesirProceduralQuestBPLibrary.h"
#include "AesirProceduralQuest.h"
#include "EditorUtilityWidget.h"
#include "Engine.h"
#include "LevelEditor.h"
#include "Quest.h"
#include "QuestFitnessBPLibrary.h"
#include "WidgetBlueprint.h"
#include "AssetRegistry/AssetRegistryModule.h"

UAesirProceduralQuestBPLibrary::UAesirProceduralQuestBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}

TArray<UObject*> UAesirProceduralQuestBPLibrary::LoadObjectLibrary(TSubclassOf<UObject> ObjectClass)
{
	TArray<UObject*> Assets;

	FAssetRegistryModule& AssetRegistryModule = FModuleManager::Get().LoadModuleChecked<FAssetRegistryModule>(AssetRegistryConstants::ModuleName);
	TArray<FAssetData> AssetDatas;
	AssetRegistryModule.Get().GetAssetsByClass(ObjectClass->GetFName(), AssetDatas);

	for (int32 i = 0; i < AssetDatas.Num(); ++i)
	{
		FAssetData& AssetData = AssetDatas[i];
		UObject* Asset = AssetData.GetAsset();
		if (Asset)
		{
			Assets.Add(AssetData.GetAsset());
			UE_LOG(LogTemp, Verbose, TEXT("%s"), *(Asset->GetName()));
		}
	}
	
	return Assets;
}

void UAesirProceduralQuestBPLibrary::DebugLogQuest(const UObject* WorldContextObject, const UQuest* Quest, const UQuestProviderPreferences* Preferences)
{
	UE_LOG(LogProceduralQuests, Verbose, TEXT("----BEGIN LOGGING QUEST----"));
	UE_LOG(LogProceduralQuests, Verbose, TEXT("Condtion Fitness   : %f"), UQuestFitnessUtils::CalculateFitnessByDesiredConditions(WorldContextObject, Quest, Preferences));
	UE_LOG(LogProceduralQuests, Verbose, TEXT("QuestLabel Fitness : %f"), UQuestFitnessUtils::CalculateFitnessByTags(Quest, Preferences));
	DebugLogAction(Quest);
	UE_LOG(LogProceduralQuests, Verbose, TEXT("----END LOGGING QUEST----"));
}

void UAesirProceduralQuestBPLibrary::DebugLogAction(const UQuestAction* Action, const int Indentation)
{
	if (!IsValid(Action))
	{
		UE_LOG(LogProceduralQuests, Error, TEXT("Can not log empty action"));
		return;
	}
	
	FString IndentationString;
	for (int Index = 0; Index < Indentation; Index++)
	{
		IndentationString.Append("    ");	
	}

	UE_LOG(LogProceduralQuests, Verbose, TEXT("%sObjective '%s'"), *IndentationString, *Action->GetName());

	IndentationString.Append("    ");

	for (const UQuestCondition* QuestCondition : Action->GetPreConditions())
	{
		DebugLogCondition("Pre", QuestCondition, Indentation + 1);
	}
	for (const UQuestCondition* QuestCondition : Action->GetPostConditions())
	{
		DebugLogCondition("Post", QuestCondition, Indentation + 1);
	}
	
	const UQuest* Quest = Cast<UQuest>(Action);
	if (IsValid(Quest))
	{
		for (const UQuestAction* QuestAction : Quest->GetActions())
		{
			DebugLogAction(QuestAction, Indentation + 1);
		}
	}
}

void UAesirProceduralQuestBPLibrary::DebugLogCondition(const FString Prefix, const UQuestCondition* Condition, const int Indentation)
{
	FString IndentationString;
	for (int Index = 0; Index < Indentation; Index++)
	{
		IndentationString.Append("    ");	
	}
	UE_LOG(LogProceduralQuests, Verbose, TEXT("%s%sCondition '%s' (%s)"), *IndentationString, *Prefix, *Condition->GetName(), *Condition->GetPropertyInfo());
}

