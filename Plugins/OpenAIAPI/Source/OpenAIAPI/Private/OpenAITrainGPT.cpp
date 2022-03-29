// Copyright Kellan Mythen 2021. All rights Reserved.


#include "OpenAITrainGPT.h"
#include "OpenAIUtils.h"
#include "Http.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

UOpenAITrainGPT* UOpenAITrainGPT::OpenAITrainGPT3(EOAEngineType engineInput, FString promptInput)
{
	UOpenAITrainGPT* BPNode = NewObject<UOpenAITrainGPT>();
	BPNode->Engine = engineInput;
	BPNode->FileId = promptInput;
	return BPNode;
}

void UOpenAITrainGPT::Activate()
{
	FString _apiKey;
	if (UOpenAIUtils::getUseApiKeyFromEnvironmentVars())
		_apiKey = UOpenAIUtils::GetEnvironmentVariable(TEXT("OPENAI_API_KEY"));
	else
		_apiKey = UOpenAIUtils::getApiKey();


	// checking parameters are valid
	if (_apiKey.IsEmpty())
	{
		Finished.Broadcast("", TEXT("Api key is not set"), false);
	} else if (FileId.IsEmpty())
	{
		Finished.Broadcast("", TEXT("Data is empty"), false);
	}



	auto HttpRequest = FHttpModule::Get().CreateRequest();
	
	
	FString APIMethod;
	switch (Engine)
	{
	case EOAEngineType::DAVINCI:
			APIMethod = "davinci";
	break;
	case EOAEngineType::CURIE:
			APIMethod = "curie";
	break;
	case EOAEngineType::BABBAGE:
			APIMethod = "babbage";
	break;
	case EOAEngineType::ADA:
			APIMethod = "ada";
	break;
	case EOAEngineType::DAVINCI_INSTRUCT_BETA:
		APIMethod = "davinci-instruct-beta";
	break;
	case EOAEngineType::CURIE_INSTRUCT_BETA:
		APIMethod = "curie-instruct-beta";
	break;
	}

	// converting parameters to strings
	const FString Header = "Bearer " + _apiKey;

	// set headers
	const FString Url = "https://api.openai.com/v1/fine-tunes";
	HttpRequest->SetURL(Url);
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	HttpRequest->SetHeader(TEXT("Authorization"), Header);

	//build payload
	TSharedPtr<FJsonObject> _payloadObject = MakeShareable(new FJsonObject());
	_payloadObject->SetStringField(TEXT("training_file"), FileId);
	_payloadObject->SetStringField(TEXT("model"), *APIMethod);

	// convert payload to string
	FString _payload;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&_payload);
	FJsonSerializer::Serialize(_payloadObject.ToSharedRef(), Writer);

	// commit request
	HttpRequest->SetVerb(TEXT("POST"));
	HttpRequest->SetContentAsString(_payload);

	if (HttpRequest->ProcessRequest())
	{
		HttpRequest->OnProcessRequestComplete().BindUObject(this, &UOpenAITrainGPT::OnResponse);
	}
	else
	{
		Finished.Broadcast("", ("Error sending request"), false);
	}


	
}

void UOpenAITrainGPT::OnResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful)
{
	if (!WasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("Error processing request. \n%s \n%s"), *Response->GetContentAsString(), *Response->GetURL());
		if (Finished.IsBound())
		{
			Finished.Broadcast("", *Response->GetContentAsString(), false);
		}

		return;
	}

	TSharedPtr<FJsonObject> responseObject;
	TSharedRef<TJsonReader<>> reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	
	if (FJsonSerializer::Deserialize(reader, responseObject))
	{
		bool err = responseObject->HasField("error");

		if (err)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s"), *Response->GetContentAsString());
			Finished.Broadcast("", *Response->GetContentAsString(), false);
			return;
		}

		Finished.Broadcast(*responseObject->GetStringField("fine-tune"), "", true);
	}
}

