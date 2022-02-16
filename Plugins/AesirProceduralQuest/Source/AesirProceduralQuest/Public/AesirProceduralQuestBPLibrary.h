// Copyright 2022 Jan Plähn. All Rights Reserved.

#pragma once

#include "QuestCondition.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "OpenAIDefinitions.h"
#include "AesirProceduralQuestBPLibrary.generated.h"

class UQuestProviderPreferences;
class UQuestCondition;
class UQuestAction;
class UQuest;
class UWidgetBlueprint;

/* 
*	
*/
UCLASS()
class AESIRPROCEDURALQUEST_API UAesirProceduralQuestBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	UFUNCTION(BlueprintCallable, Category = "Procedural Quest Library")
	static TArray<UObject*> LoadObjectLibrary(TSubclassOf<UObject> ObjectClass);

	UFUNCTION(BlueprintCallable, Category = "Procedural Quest Library")
	static void DebugLogQuest(const UObject* WorldContextObject, UQuest* Quest, const UQuestProviderPreferences* Preferences);

	static void DebugLogAction(const UQuestAction* Action, const int Indentation = 0);

	static void DebugLogCondition(const FString Prefix, const UQuestCondition* Condition, const int Indentation = 0);
	
	static void InjectNameParameter(FName& InOutName, const TArray<UQuestParameter*>& Parameters);

	UFUNCTION(BlueprintCallable, Category = "Procedural Quest Library")
	static FString CreateOpenAiPrompt(const UQuest* Quest);

	UFUNCTION(BlueprintCallable, Category = "Procedural Quest Library")
	static void LogOpenAiResponses(const TArray<FCompletion> Completions);
	
	template <class T>
	static float GetListSimilarity(const TArray<T> SetA, const TArray<T> SetB)
	{		
		const float Max = FMath::Max(SetA.Num(), SetB.Num());
		if (Max == 0)
		{
			return 1.0f;
		}

		//calculate intersection num
		const bool bBSmaller = (SetA.Num() > SetB.Num());
		const TArray<T>& A = (bBSmaller ? SetB : SetA);
		const TArray<T>& B = (bBSmaller ? SetA : SetB);

		int IntersectionNum = 0;
		for (const T Element : A)
		{
			if (B.Contains(Element))
			{
				IntersectionNum++;
			}
		}
		
		return IntersectionNum / Max;
	};

	static float GetActionListSimilarity(const TArray<UQuestCondition*> SetA, const TArray<UQuestCondition*> SetB)
	{		
		const float Max = FMath::Max(SetA.Num(), SetB.Num());
		if (Max == 0)
		{
			return 1.0f;
		}

		//calculate intersection num
		const bool bBSmaller = (SetA.Num() > SetB.Num());
		const TArray<UQuestCondition*>& A = (bBSmaller ? SetB : SetA);
		const TArray<UQuestCondition*>& B = (bBSmaller ? SetA : SetB);

		int IntersectionNum = 0;
		for (const UQuestCondition* Element : A)
		{
			if(B.ContainsByPredicate([Element](UQuestCondition* Condition){ return Element->GetId() == Condition->GetId();}))
			{
				IntersectionNum++;
			}
			if (B.Contains(Element))
			{
				IntersectionNum++;
			}
		}
		
		return IntersectionNum / Max;
	};
};
