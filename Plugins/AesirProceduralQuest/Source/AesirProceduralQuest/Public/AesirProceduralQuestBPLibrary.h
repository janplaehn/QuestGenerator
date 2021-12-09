// Copyright 2022 Jan Plähn. All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "AesirProceduralQuestBPLibrary.generated.h"

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
};
