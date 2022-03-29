// Copyright Kellan Mythen 2021. All rights Reserved.


#include "OpenAIUploadFile.h"
#include "OpenAIUtils.h"
#include "Http.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"

UOpenAIUploadFile* UOpenAIUploadFile::OpenAICreateFile(FString InPath, EOAPurposeType InPurpose)
{
	UOpenAIUploadFile* BPNode = NewObject<UOpenAIUploadFile>();
	BPNode->Path = InPath;
	BPNode->Purpose = InPurpose;
	return BPNode;
}

TArray<uint8> FStringToUint8(const FString& InString)
{
	TArray<uint8> OutBytes;

	// Handle empty strings
	if (InString.Len() > 0)
	{
		FTCHARToUTF8 Converted(*InString); // Convert to UTF8
		OutBytes.Append(reinterpret_cast<const uint8*>(Converted.Get()), Converted.Length());
	}

	return OutBytes;
}

void UOpenAIUploadFile::Activate()
{
	FString _apiKey;
	if (UOpenAIUtils::getUseApiKeyFromEnvironmentVars())
		_apiKey = UOpenAIUtils::GetEnvironmentVariable(TEXT("OPENAI_API_KEY"));
	else
		_apiKey = UOpenAIUtils::getApiKey();


	// checking parameters are valid
	if (_apiKey.IsEmpty())
	{
		Finished.Broadcast(TEXT("Api key is not set"), false);
	} else if (Path.IsEmpty())
	{
		Finished.Broadcast(TEXT("Path is empty"), false);
	}



	auto HttpRequest = FHttpModule::Get().CreateRequest();
	
	
	FString PurposeId;
	switch (Purpose)
	{
	case EOAPurposeType::SEARCH:
			PurposeId = "search";
	break;
	case EOAPurposeType::ANSWERS:
			PurposeId = "answers";
	break;
	case EOAPurposeType::CLASSIFICATIONS:
			PurposeId = "classifications";
	break;
	case EOAPurposeType::FINE_TUNE:
			PurposeId = "fine-tune";
	break;
	}

	// converting parameters to strings
	const FString Header = "Bearer " + _apiKey;
	const FString FormattedPath = "@" + Path;
	const FString Boundary(TEXT("------------------------f2a4a742c08bf427"));
	const FString PrefixBoundary(TEXT("\r\n--") + Boundary + TEXT("\r\n"));
	const FString SuffixBoundary(TEXT("\r\n--") + Boundary + TEXT("--\r\n"));

	const FString FormData = FString::Printf(TEXT("Content-Disposition: form-data; purpose=\"%s\"; file=\"@%s\"\r\n"), *PurposeId, *Path);
	//const FString fileHeader(TEXT("Content-Disposition: form-data; name=\"upload_file_minidump\"; filename=\"UE4Minidump.dmp\"\r\nContent-Type: application/octet-stream\r\n\r\n"));

	const FString Content(PrefixBoundary + FormData /*+ fileContents*/ + SuffixBoundary);
	
	// set headers
	const FString Url = "https://api.openai.com/v1/files";
	HttpRequest->SetURL(Url);
	//HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	HttpRequest->SetHeader("Authorization", Header);
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("multipart/form-data; boundary=") + Boundary);
	//HttpRequest->SetHeader("Content-Disposition", *FormData);

	TArray<uint8> CombinedContent;
	CombinedContent.Append(FStringToUint8(PrefixBoundary + FormData));
	//CombinedContent.Append(dumpFileData);
	CombinedContent.Append(FStringToUint8(SuffixBoundary));
	
	HttpRequest->SetContent(CombinedContent);

	//build payload
	//TSharedPtr<FJsonObject> _payloadObject = MakeShareable(new FJsonObject());
	//_payloadObject->SetStringField(TEXT("purpose"), *PurposeId);
	//_payloadObject->SetStringField(TEXT("file"), *Path);

	//const FString fileName(FPaths::Combine(path, crashToReport.folderName, TEXT("UE4Minidump.dmp")));

	//ensure(FPaths::FileExists(fileName));

	//const FString fileHeader(TEXT("Content-Disposition: form-data; name=\"upload_file_minidump\"; filename=\"UE4Minidump.dmp\"\r\nContent-Type: application/octet-stream\r\n\r\n"));

	//FString fileContents;
	//FFileHelper::LoadFileToString(fileContents, *fileName);

	// convert payload to string
	//FString _payload;
	//TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&_payload);
	//FJsonSerializer::Serialize(_payloadObject.ToSharedRef(), Writer);

	// commit request
	HttpRequest->SetVerb(TEXT("POST"));
	//HttpRequest->SetContentAsString(_payload);

	if (HttpRequest->ProcessRequest())
	{
		HttpRequest->OnProcessRequestComplete().BindUObject(this, &UOpenAIUploadFile::OnResponse);
	}
	else
	{
		Finished.Broadcast("Error sending request", false);
	}


	
}

void UOpenAIUploadFile::OnResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool WasSuccessful)
{
	if (!WasSuccessful)
	{
		UE_LOG(LogTemp, Warning, TEXT("Error processing request. \n%s \n%s"), *Response->GetContentAsString(), *Response->GetURL());
		if (Finished.IsBound())
		{
			Finished.Broadcast(*Response->GetContentAsString(), false);
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
			Finished.Broadcast(*Response->GetContentAsString(), false);
			return;
		}

		Finished.Broadcast("", true);
	}
}

