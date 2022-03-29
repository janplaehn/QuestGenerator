// Copyright Kellan Mythen 2021. All rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "OpenAIDefinitions.h"
#include "HttpModule.h"
#include "OpenAIUploadFile.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnUploadResponseRecievedPin, const FString&, errorMessage, bool, Success);

/**
 * 
 */
UCLASS()
class OPENAIAPI_API UOpenAIUploadFile : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()


public:
	EOAPurposeType Purpose = EOAPurposeType::FINE_TUNE;
	FString Path = "";

	UPROPERTY(BlueprintAssignable, Category = "OpenAI")
	FOnUploadResponseRecievedPin Finished;

private:
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"), Category = "OpenAI")
	static UOpenAIUploadFile* OpenAICreateFile(FString InPath, EOAPurposeType InPurpose);

	virtual void Activate() override;
	void OnResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful);
};