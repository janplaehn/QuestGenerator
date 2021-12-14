// Copyright 2022 Jan Plähn. All Rights Reserved.

#include "AesirProceduralQuestBPLibrary.h"
#include "AesirProceduralQuest.h"
#include "EditorUtilityWidget.h"
#include "Engine.h"
#include "LevelEditor.h"
#include "Quest.h"
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

void UAesirProceduralQuestBPLibrary::DebugLogQuest(const UQuest* Quest)
{
	UE_LOG(LogProceduralQuests, Verbose, TEXT("----BEGIN LOGGING QUEST----"));
	DebugLogAction(Quest);
	UE_LOG(LogProceduralQuests, Verbose, TEXT("----END LOGGING QUEST----"));
}

void UAesirProceduralQuestBPLibrary::DebugLogAction(const UQuestAction* Action, const int Indentation)
{
	FString IndentationString;
	for (int Index = 0; Index < Indentation; Index++)
	{
		IndentationString.Append("    ");	
	}

	UE_LOG(LogProceduralQuests, Verbose, TEXT("%sObjective '%s'"), *IndentationString, *Action->GetName());

	IndentationString.Append("    ");

	for (const UQuestCondition* QuestCondition : Action->GetPreConditions())
	{
		DebugLogPreCondition(QuestCondition, Indentation + 1);
	}
	for (const UQuestCondition* QuestCondition : Action->GetPostConditions())
	{
		DebugLogPreCondition(QuestCondition, Indentation + 1);
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

void UAesirProceduralQuestBPLibrary::DebugLogPreCondition(const UQuestCondition* Condition, const int Indentation)
{
	FString IndentationString;
	for (int Index = 0; Index < Indentation; Index++)
	{
		IndentationString.Append("    ");	
	}
	UE_LOG(LogProceduralQuests, Verbose, TEXT("%sPreCondition '%s'"), *IndentationString, *Condition->GetName());
}

void UAesirProceduralQuestBPLibrary::DebugLogPostCondition(const UQuestCondition* Condition, const int Indentation)
{
	FString IndentationString;
	for (int Index = 0; Index < Indentation; Index++)
	{
		IndentationString.Append("    ");	
	}
	UE_LOG(LogProceduralQuests, Verbose, TEXT("%sPostCondition '%s'"), *IndentationString, *Condition->GetName());
}

