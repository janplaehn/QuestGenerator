// Copyright 2022 Jan Plähn. All Rights Reserved.

#include "AesirProceduralQuest.h"

#if WITH_UNREAL_DEVELOPER_TOOLS
#include "Developer/MessageLog/Public/MessageLogModule.h"
#endif

#define LOCTEXT_NAMESPACE "FAesirProceduralQuestModule"

DEFINE_LOG_CATEGORY(LogProceduralQuests);

void FAesirProceduralQuestModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
}

void FAesirProceduralQuestModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FAesirProceduralQuestModule, AesirProceduralQuest)