// Copyright 2022 Jan Plähn. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "AesirProceduralQuestBPLibrary.generated.h"

class UQuestCondition;
class UQuestAction;
class UQuest;
class UWidgetBlueprint;

/* 
*	
*/
UCLASS()
class UAesirProceduralQuestBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

	UFUNCTION(BlueprintCallable, Category = "Procedural Quest Library")
	static TArray<UObject*>  LoadObjectLibrary(TSubclassOf<UObject> ObjectClass);

	static void DebugLogQuest(const UQuest* Quest);

	static void DebugLogAction(const UQuestAction* Action, const int Indentation = 0);

	static void DebugLogCondition(const FString Prefix, const UQuestCondition* Condition, const int Indentation = 0);
};
