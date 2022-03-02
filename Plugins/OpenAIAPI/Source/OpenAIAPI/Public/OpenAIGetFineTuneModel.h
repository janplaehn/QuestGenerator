// Copyright Kellan Mythen 2021. All rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "OpenAIDefinitions.h"
#include "HttpModule.h"
#include "OpenAITrainGPT.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnTrainingResponseRecievedPin, const FString&, fineTuneJobId, const FString&, errorMessage, bool, Success);

/**
 * 
 */
UCLASS()
class OPENAIAPI_API UOpenAITrainGPT : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()


public:
	EOAEngineType Engine = EOAEngineType::DAVINCI;
	FString FileId = "";

	UPROPERTY(BlueprintAssignable, Category = "OpenAI")
	FOnTrainingResponseRecievedPin Finished;

private:
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "OpenAI")
	static UOpenAITrainGPT* OpenAITrainGPT3(EOAEngineType engine, FString data);

	virtual void Activate() override;
	void OnResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful);
};