// Copyright Epic Games, Inc. All Rights Reserved.

#include "AesirProceduralQuestEditorCommands.h"

#define LOCTEXT_NAMESPACE "FAesirProceduralQuestEditorModule"

void FAesirProceduralQuestEditorCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "Quests", "Bring up AesirProceduralQuestEditor window", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
