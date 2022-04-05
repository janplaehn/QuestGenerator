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

void UAesirProceduralQuestBPLibrary::InjectNameParameter(FName& InOutName, const TArray<UQuestParameter*>& Parameters)
{
	const auto FoundParameter = Parameters.FindByPredicate([InOutName](UQuestParameter* Parameter)
	{
		return InOutName == Parameter->GetParameterName();
	});

	if (FoundParameter == nullptr)
	{
		return;
	}

	InOutName = (*FoundParameter)->GetValueAsName();
}

void UAesirProceduralQuestBPLibrary::DebugLogQuest(const UObject* WorldContextObject, UQuest* Quest, const UQuestProviderPreferences* Preferences)
{
	UE_LOG(LogProceduralQuests, Verbose, TEXT("----BEGIN LOGGING QUEST----"));
	UE_LOG(LogProceduralQuests, Verbose, TEXT("Condtion Fitness   : %f"), UQuestFitnessUtils::CalculateFitnessByDesiredConditions(WorldContextObject, Quest, Preferences));
	UE_LOG(LogProceduralQuests, Verbose, TEXT("Intentionality Fitness : %f"), UQuestFitnessUtils::CalculateFitnessByIntentionality(Quest));
	UE_LOG(LogProceduralQuests, Verbose, TEXT("Affinity Fitness : %f"), UQuestFitnessUtils::CalculateFitnessByAffinity(Quest, Preferences));
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

	UE_LOG(LogProceduralQuests, Verbose, TEXT("%sObjective '%s'"), *IndentationString, *Action->GetDescription().ToString());

	IndentationString.Append("    ");

	// for (const UQuestCondition* QuestCondition : Action->GetPreConditions())
	// {
	// 	DebugLogCondition("Pre", QuestCondition, Indentation + 1);
	// }
	// for (const UQuestCondition* QuestCondition : Action->GetPostConditions())
	// {
	// 	DebugLogCondition("Post", QuestCondition, Indentation + 1);
	// }
	
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


FString UAesirProceduralQuestBPLibrary::CreateOpenAiPrompt(const UQuest* Quest)
{
	FString OutString;
	OutString += "Describe the following game-quest. Do not use bullet points.";
	OutString += Quest->GetProviderData()->ProviderName.ToString();
	OutString += ":";
	OutString += LINE_TERMINATOR;

	const TArray<const UQuestAction*>& Actions = Quest->GetActions();
	
	for (int I = 0; I < Actions.Num(); I++)
	{
		OutString += LINE_TERMINATOR;
		OutString += FString::Printf(TEXT("%d"),I) + ". ";
		OutString += Actions[I]->GetDescription().ToString();
	}
	return OutString;
}

void UAesirProceduralQuestBPLibrary::LogOpenAiResponses(const TArray<FCompletion> Completions)
{
	for (const FCompletion& Completion: Completions)
	{
		FString Output = Completion.text.Replace(TEXT("\r"),TEXT("")).Replace(TEXT("\n"),TEXT(""));	
		UE_LOG(LogProceduralQuests, Verbose, TEXT("OpenAI completion received: '%s'"), *Completion.text);
	}
}

